#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "zlib.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/time.h>
struct timeval start, stop, delta;

double calTime()
{
   struct timeval time;
   gettimeofday(&time, NULL);
   
   double calTime = (time.tv_sec - start.tv_sec) * 1000.0;
   calTime += (time.tv_usec - start.tv_usec)/1000.0;
   return calTime;
}

int main(int argc,char **argv)
{
    int sockfd,n;
    char sendline[1024];
    char recvline[1024];
    char file_name[256];
    int filesize;
    int recv_file_size;
    int remain;
    int bytesReceived;
    struct sockaddr_in servaddr;
    struct stat file_stat;

    if(argc != 4){
    	printf("\n Error : Invalid Input \n");
        return 1;
    }
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[2]));
 
    inet_pton(AF_INET,argv[1],&(servaddr.sin_addr));
 
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    strcpy(sendline, argv[3]);
    //send file name
    write(sockfd, sendline, strlen(sendline)+1);

    FILE *fd=fopen(argv[3],"rb");
    if(fd == NULL){
    	printf("File open error");
        return 1;
    }

	/* Get file stats */
    fseek (fd, 0L, SEEK_END); 
    filesize = ftell(fd);
    fseek (fd, 0L, SEEK_SET);
    sprintf(sendline, "%d", filesize);


    //send file size
    write(sockfd, sendline, strlen(sendline)+1); 
    printf("Total size of the file is %s\n",sendline);  

    printf("Start sending the file\n");
    gettimeofday(&start,NULL);
    while(1){
    	bzero(sendline, 1024);
    	int nread = fread(sendline,1,1024,fd);
    	if(nread < 1024){
       	    int n = write(sockfd,sendline,nread);
            if(n<0){
                perror("Problem sendto\n");
            }
        }else{
        	int n = write(sockfd,sendline,1024);
        }
   		
    	if (feof(fd)){
    		printf("Finish sending the file\n");
            break;
        }

        if (ferror(fd))
            printf("Error reading the file at client program\n");
    	    	     	
    } 

    fclose(fd);

    //Received the file name
    bzero(recvline, 1024);
    read(sockfd,recvline,1024);
    strcpy(file_name, recvline);

    FILE *fp = fopen(file_name, "w");
    if(NULL == fp){
        printf("Error opening the file");
        exit(EXIT_FAILURE);
    }

    //Receving unzipped file back
    bzero(recvline, 1024);
    read(sockfd,recvline,1024);
    recv_file_size = atoi(recvline);
    remain = recv_file_size;
    printf("Received the total size of the file: %d\n",recv_file_size); 

    printf("Start receiving the file\n");
    while(remain > 0){ 
        bzero(recvline, 1024); 
        bytesReceived=read(sockfd,recvline,1024); 
        if (bytesReceived<0){
            printf("Recvfrom: Error in recieving the file\n");
        }

        if(fwrite(recvline,1,bytesReceived,fp)<0){
            printf("Error writting the file\n");
        }

        remain -= bytesReceived;
         
    }

    printf("Finish receving the file\n");
    printf("The total duration is: %.2fms\n",calTime());
    fclose(fp);

    return 1;
 
}