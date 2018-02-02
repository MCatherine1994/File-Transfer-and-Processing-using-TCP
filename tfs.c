 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zlib.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
 
int main(int argc,char **argv)
{
 
    char receive_buffer[1024];
    char send_buffer[1024];
    char file_name[1024];
    int file_size;
    int send_file_size;
    int remain_size;
    int listen_fd, comm_fd, bytesReceived;
    char command[256] = "unzip ";

    if(argc != 3){
        printf("\n Error : Invalid Input \n");
        return 1;
    }
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(listen_fd, 10);
 
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    //receive file name
    read(comm_fd,receive_buffer,1024);
    strcpy(file_name,receive_buffer);

    //recevie file size
    bzero(receive_buffer, 1024);
    read(comm_fd,receive_buffer,1024);
    file_size = atoi(receive_buffer);
    remain_size = file_size;
    printf("Received the total size of the file: %d\n",file_size);
  
    FILE *fp=fopen(file_name, "wb");
    if(NULL == fp){
        printf("Error opening the file");
        exit(EXIT_FAILURE);
    }
    
    printf("Start receiving the file\n");
    while(remain_size > 0){ 
        bzero(receive_buffer, 1024); 
        bytesReceived=read(comm_fd,receive_buffer,1024); 
        if (bytesReceived<0){
            printf("Recvfrom: Error in recieving the file\n");
        }

        if(fwrite(receive_buffer,1,bytesReceived,fp)<0){
            printf("Error writting the file\n");
        }

        remain_size -= bytesReceived;
         
    }

    printf("Finish receving the file\n");
    fclose(fp);

    //Unzip the received file
    printf("Unzip the file\n");
    strcat(command, file_name);
    system(command);

    //Send unzipped file_name
    file_name[strlen(file_name)-4] = '\0';
    printf("%s\n", file_name);
    strcat(file_name, ".txt");
    printf("%s\n", file_name);
    bzero(send_buffer, 1024);
    strcpy(send_buffer, file_name);
    write(comm_fd,send_buffer,1024);

    //sending the unziped file back
    FILE *fd = fopen(file_name, "r");
    if(fd == NULL){
        printf("Error opening the file");
        exit(EXIT_FAILURE);
    }

    //send file size
    bzero(send_buffer, 1024); 
    fseek (fd, 0L, SEEK_END); 
    send_file_size = ftell(fd);
    fseek (fd, 0L, SEEK_SET);
    sprintf(send_buffer, "%d", send_file_size);
    printf("Sending back the file size: %d\n", send_file_size);
    write(comm_fd,send_buffer,1024);

    printf("Start sending the unzipped file\n");
    while(!feof(fd)){
        bzero(send_buffer, 1024); 
        int nread = fread(send_buffer,1,1024,fd);
        if(nread < 1024){
            int n = write(comm_fd,send_buffer,nread);
            if(n<0){
                perror("Problem sendto\n");
            }
        }else{
            int n = write(comm_fd,send_buffer,1024);
        }

        if (ferror(fd))
            printf("Error reading the file at client program\n");

    }

    printf("Finish sending the unzipped file\n");
    fclose(fd);
    
    return 1;
}    
