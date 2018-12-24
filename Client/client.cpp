#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace std;

#define MAXSIZE 1024

int createSocket(int, char*);
int acceptConection(int);

int main(int argc, char** argv) {
  char cmd[MAXSIZE], recvline[MAXSIZE];

  /* The port number */
  int port = -1;

  /* The file descriptor representing the connection to the client */
  int connfd = -1;

  /* The number of bytes sent in one shot */
  int numSent = 0;

  string response;

  /* The total number of bytes sent */
  off_t totalNumSent = 0;

  /* The size of the file name */
  int fileNameSize = -1;

  /* The structures representing the server address */
  sockaddr_in serverAddr;

  /* Stores the size of the client's address */
  socklen_t servLen = sizeof(serverAddr);

  /* Make sure the user has provided the port number to
	 * listen on
	 */
  if (argc < 3 || argc > 3) {
    /* Report an error */
    fprintf(stderr, "USAGE: %s <SERVER IP> <SERVER PORT #>", argv[0]);
    exit(-1);
  }

  /* Get the port number */
  port = atoi(argv[2]);

  /* Make sure that the port is within a valid range */
  if (port < 0 || port > 65535) {
    fprintf(stderr, "Invalid port number\n");
    exit(-1);
  }

  /* Connect to the server */
  if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(-1);
  }

  /* Set the structure to all zeros */
  memset(&serverAddr, 0, sizeof(serverAddr));

  /* Set the server family */
  serverAddr.sin_family = AF_INET;

  /* Convert the port number to network representation */
  serverAddr.sin_port = htons(port);

  /**
	 * Convert the IP from the presentation format (i.e. string)
	 * to the format in the serverAddr structure.
	 */
  if (!inet_pton(AF_INET, argv[1], &serverAddr.sin_addr)) {
    perror("inet_pton");
    exit(-1);
  }

  /* Lets connect to the client. This call will return a socket used 
	 * used for communications between the server and the client.
	 */
  if (connect(connfd, (sockaddr*)&serverAddr, sizeof(sockaddr)) < 0) {
    perror("connect");
    exit(-1);
  }

  cout << "ftp> ";

  while (fgets(cmd, MAXSIZE, stdin) != NULL) {
    send(connfd, cmd, MAXSIZE, 0);
    char* input;
    input = strtok(cmd, " ");

    if (strcmp("get", input) == 0) {
      char port[MAXSIZE], buf[MAXSIZE], msg[MAXSIZE], charFileSizeStart[MAXSIZE], charFileSizeEnd[MAXSIZE];
      FILE* file;

      recv(connfd, port, MAXSIZE, 0);

      int data = createSocket(atoi(port), argv[1]);

      input = strtok(NULL, " \n");

      recv(connfd, msg, MAXSIZE, 0);

      // checks if the input has a command and filename
      if (strcmp("1", msg) == 0) {
        if ((file = fopen(input, "w+")) == NULL) {
          cout << "Error writing the file.\n";
        } else {
          recv(connfd, charFileSizeStart, MAXSIZE, 0);

          for (int i = 0; i < 0; i++) {
            recv(data, buf, MAXSIZE, 0);
            fwrite(buf, sizeof(char), MAXSIZE, file);
          }

          recv(connfd, charFileSizeEnd, MAXSIZE, 0);
          int fileSize = atoi(charFileSizeEnd);

          if (fileSize > 0) {
            recv(data, buf, MAXSIZE, 0);
            fwrite(buf, sizeof(char), fileSize, file);
          }

          fclose(file);
          cout << "Downloaded file from server.\n";
        }
      } else {
        cout << "Error. File doesn't exist or invalid input. Usage: get <filename>.";
      }
    } else if (strcmp("put", input) == 0) {
      char port[MAXSIZE], buf[MAXSIZE], charFileSizeStart[MAXSIZE], charFileSizeEnd[MAXSIZE];

      FILE* file;

      recv(connfd, port, MAXSIZE, 0);

      int data = createSocket(atoi(port), argv[1]);

      input = strtok(NULL, " \n");

      if ((file = fopen(input, "r")) != NULL) {
        send(connfd, "1", MAXSIZE, 0);

        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);

        int fileSizeStart = 0;
        int fileSizeEnd = size % MAXSIZE;

        sprintf(charFileSizeStart, "%d", fileSizeStart);
        send(connfd, charFileSizeStart, MAXSIZE, 0);

        for (int i = 0; i < fileSizeStart; i++) {
          fread(buf, sizeof(char), MAXSIZE, file);
          send(data, buf, MAXSIZE, 0);
        }

        sprintf(charFileSizeEnd, "%d", fileSizeEnd);
        send(connfd, charFileSizeEnd, MAXSIZE, 0);

        if (fileSizeEnd > 0) {
          fread(buf, sizeof(char), fileSizeEnd, file);
          send(data, buf, MAXSIZE, 0);
        }

        fclose(file);
        cout << "Uploaded file to server.\n";
      }
    } else if (strcmp("ls\n", cmd) == 0) {
      char port[MAXSIZE], buf[MAXSIZE], temp[MAXSIZE] = "1";

      recv(connfd, port, MAXSIZE, 0);

      int data = createSocket(atoi(port), argv[1]);

      while (strcmp("1", temp) == 0) {
        recv(data, temp, MAXSIZE, 0);

        if (strcmp("0", temp) == 0) {
          break;
        }

        recv(data, buf, MAXSIZE, 0);
        cout << buf;
      }
    } else if (strcmp("quit\n", cmd) == 0) {
      return 0;
    } else {
      cout << "Invalid command. Commands: get, put, ls, quit.\n";
    }

    cout << "ftp> ";
  }

  exit(0);
}

int getFileSize(const char* fileName) {
  /* The structure containing the file information */
  struct stat fileInfo;

  /* Get the file size */
  if (stat(fileName, &fileInfo) < 0)
    return -1;

  /* Return the file size */
  return fileInfo.st_size;
}

// function for creating a socket for data connection
int createSocket(int port, char* serverIP) {
  int connfd = -1;

  sockaddr_in serverAddr;

  /* Connect to the server */
  if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(-1);
  }

  /* Set the structure to all zeros */
  memset(&serverAddr, 0, sizeof(serverAddr));

  /* Set the server family */
  serverAddr.sin_family = AF_INET;

  serverAddr.sin_addr.s_addr = inet_addr(serverIP);

  /* Convert the port number to network representation */
  serverAddr.sin_port = htons(port);

  /* Lets connect to the client. This call will return a socket used 
	 * used for communications between the server and the client.
	 */
  if (connect(connfd, (sockaddr*)&serverAddr, sizeof(sockaddr)) < 0) {
    perror("connect");
    exit(-1);
  }

  return (connfd);
}

int acceptConection(int connfd) {
  int dConnfd;
  sockaddr_in dCliAddr;
  socklen_t dCliLen = sizeof(dCliAddr);

  /* A structure to store the client address */
  if ((dConnfd = accept(connfd, (sockaddr*)&dCliAddr, &dCliLen)) < 0) {
    perror("accept");
    exit(-1);
  }

  return 0;
}