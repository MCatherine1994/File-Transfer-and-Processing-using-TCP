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
Run the client side: $./tfc <ip_add> <port_num> <zip_file>   

### **Further Discussion using WireShark**:   
**1)** As the first set of results, make sure that the router has no prior configuration to drop the packets and no loss occurs during the file transfer process at the router. You may use ‘tc qdisc show’ at the router to check the existing configuration. Then, use Wireshark to capture the transferred traffic between two hosts at outgoing interface of router. After finishing the file transfer, use the captured traffic to count the number of transmitted/received packets at both hosts.  

**2)** In the next step,set the packet drop ratio to 20% (using tccommand) at the router and then run Wireshark to capture the traffic at the ongoing interface of the router. After finishing the file transfer process, count the total number of transmitted/received packets at the both hosts, again. Also, count the total number of retransmitted packets (by the both hosts) for this scenario. You can either use Wireshark filters or your own client/server programs to do this task.  

**3)** After getting the results for the explained scenarios, write a short and concise analysis and justify your observations and the outcomes. You can use proper snapshots and graphs to elaborate your justification in the report.  

**Check the report for answers.**

### **Reference on git**  
http://dont-be-afraid-to-commit.readthedocs.io/en/latest/git/commandlinegit.html
