# CPSC 471 Programming Assignment 1

Simple FTP server and client implementation in C++

### Members
* Himanshu Patel - himanshup@csu.fullerton.edu
* Justin Nhek - jnhek0@csu.fullerton.edu
* Abdullah Zubair - malik@csu.fullerton.edu
* Wellson Pan - WellsonPan@csu.fullerton.edu
* Jeremy Gibbons - jeremymgibbons@gmail.com

### How to execute:

To execute the program, open up two terminals, change directory to the Client folder in one of the terminals, and then change directory to the Server folder in the other terminal. Then enter the folllowing commands:  

Server side:
```
g++ server.cpp -o server (if you need to compile)
./server <port #>
```

Client side:  
```
g++ client.cpp -o client (if you need to compile)
./client <ip address of server> <port #>
```

Commands for program:  
```
put <filename> (uploads file <file name> to the server)
get <filename> (downloads file <file name> from the server)
ls (lists files on the server)
quit (disconnects from the server and exits)
```

### Comments
* The get command will download the file from the server but the text in the file will be missing.
* After entering the get command and downloading the file from the server, it won't let you enter any more commands after that (still trying to fix this, only happens with get command)
