## **File Transfer and Processing using TCP**  

### **Requirement**:   
Develop a client/server program for file transfer/processing based on Stream (TCP) sockets. The input arguments of server program are an IP address and Port number. The client program has 3 input arguments including the IP address and Port number of the server side as well as the Name of a zipped file.

### **Steps**:
1) Read the given zip file and send it to the server program at host-b.
2) Unzip the received file & send it back to the client.
3) Client side program displays the total duration of the file transfer process in milliseconds.
4) Check the checksum of the original file and the received file (using md5sum Linux command).  

### **Commands**:  
Compile: $gcc tfs.c -o tfs  
Run the server side: $./tfs <ip_add> <port_num>  
Complie: $gcc tfc.c -o tfc    
Run the client side: $./tfc ip_add port_num <zip_file>   

### **Further Discussion using WireShark**:   
**1)** As the first set of results, make sure that the router has no prior configuration to drop the packets and no loss occurs during the file transfer process at the router. You may use ‘tc qdisc show’ at the router to check the existing configuration. Then, use Wireshark to capture the transferred traffic between two hosts at outgoing interface of router. After finishing the file transfer, use the captured traffic to count the number of transmitted/received packets at both hosts.  

**Answer**:  
Total number of transmitted packets at hostA is 1239.
Total number of transmitted packets at hostB is 1867.
The total number of transmitted/received packets at both sides are 3108.  

**2)** Inthenextstep,setthepacketdropratioto20%(usingtccommand)attherouter and then run Wireshark to capture the traffic at the ongoing interface of the router. After finishing the file transfer process, count the total number of transmitted/received packets at the both hosts, again. Also, count the total number of retransmitted packets (by the both hosts) for this scenario. You can either use Wireshark filters or your own client/server programs to do this task.  

**Answer**:  
$tc qdisc add dev ens6 root netem drop 20%  
Total number of transmitted packets at hostA is 2125.  
Total number of transmitted packets at hostB is 3987.  
The total number of transmitted/received packets at both sides are 6143.  

Total number of retransmitted packets at hostA is 105.

Total number of retransmitted packets at hostB is 11.  

The total number of retransmitted packets at both sides are 116.

**3)** After getting the results for the explained scenarios, write a short and concise analysis and justify your observations and the outcomes. You can use proper snapshots and graphs to elaborate your justification in the report.  

**Answer**:  
When there is no drop request, the TCP Stream starts at 0 and end almost around 6, which is almost the same as the time accounted by the program. Could see congestion control around 0.4 where there are duplicate ACKs at packet 1776. Flow control could bee seen at packet 2423 where the server’s window size is 0.  

When there is drop request, the TCP Stream starts at 0 and end almost around 21, which is almost the same as the time accounted by the program. It lasts for longer time because of retransmission of dropped packets. Also could see the retransmission packets, like from 0.2s to 3s, it is retransmitting, which could also been see from the captured retransmitted packets. Flow control and congestion control are same as previous one, so no explanation here.


