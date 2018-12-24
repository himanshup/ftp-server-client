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

int main(int argc, char **argv) {
  char cmd[MAXSIZE];
  int n, portNum = 1024;

  /* The port number */
  int port = -1;

  /* The integer to store the file descriptor number
	 * which will represent a socket on which the server
	 * will be listening
	 */
  int listenfd = -1;

  /* The file descriptor representing the connection to the client */
  int connfd = -1;

  /* The structures representing the server and client
	 * addresses respectively
	 */
  sockaddr_in serverAddr, cliAddr;

  /* Stores the size of the client's address */
  socklen_t cliLen = sizeof(cliAddr);

  /* Make sure the user has provided the port number to
	 * listen on
	 */
  if (argc < 2) {
    /* Report an error */
    fprintf(stderr, "USAGE: %s <PORT #>", argv[0]);
    exit(-1);
  }

  /* Get the port number */
  port = atoi(argv[1]);

  /* Make sure that the port is within a valid range */
  if (port < 0 || port > 65535) {
    fprintf(stderr, "Invalid port number\n");
    exit(-1);
  }

  /* Print the port number */
  fprintf(stderr, "Port  = %d\n", port);

  /* Create a socket that uses
	 * IPV4 addressing scheme (AF_INET),
	 * Supports reliable data transfer (SOCK_STREAM),
	 * and choose the default protocol that provides
	 * reliable service (i.e. 0); usually TCP
	 */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(-1);
  }

  /* Set the structure to all zeros */
  memset(&serverAddr, 0, sizeof(serverAddr));

  /* Convert the port number to network representation */
  serverAddr.sin_port = htons(port);

  /* Set the server family */
  serverAddr.sin_family = AF_INET;

  /* Retrieve packets without having to know your IP address,
	 * and retrieve packets from all network interfaces if the
	 * machine has multiple ones
	 */
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Associate the address with the socket */
  if (bind(listenfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("bind");
    exit(-1);
  }

  /* Listen for connections on socket listenfd.
	 * allow no more than 100 pending clients.
	 */
  if (listen(listenfd, 100) < 0) {
    perror("listen");
    exit(-1);
  }

  /* Wait forever for connections to come */
  fprintf(stderr, "Waiting for somebody to connect on port %d\n", port);

  while (true) {
    /* A structure to store the client address */
    if ((connfd = accept(listenfd, (sockaddr *)&cliAddr, &cliLen)) < 0) {
      perror("accept");
      exit(-1);
    }

    fprintf(stderr, "Connected!\n");

    while ((n = recv(connfd, cmd, MAXSIZE, 0)) > 0) {
      cout << "Input from client: " << cmd;
      char *fileName;
      fileName = strtok(cmd, " ");

      if (strcmp("quit\n", cmd) == 0) {
        cout << "Client has quit\n";
      }

      if (strcmp("ls\n", cmd) == 0) {
        cout << "ls command\n";
      }

      if (strcmp("get", fileName) == 0) {
        char port[MAXSIZE], buf[MAXSIZE];
        int sock;

        fileName = strtok(NULL, " \n");
        cout << "Filename: " << fileName << endl;

        FILE *fp;
        portNum += 1;

        if (portNum == atoi(argv[1])) {
          portNum += 1;
        }

        sock = createSocket(portNum);
        send(connfd, port, MAXSIZE, 0);

        sock = acceptConection(sock);

        // still working on this
      }

      if (strcmp("put", fileName) == 0) {
      }
    }

    close(connfd);
  }
}

int createSocket(int port) {
  int listenfd = -1;

  sockaddr_in serverAddr;

  /* Connect to the server */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(-1);
  }

  /* Set the structure to all zeros */
  memset(&serverAddr, 0, sizeof(serverAddr));

  /* Convert the port number to network representation */
  serverAddr.sin_port = htons(port);

  /* Set the server family */
  serverAddr.sin_family = AF_INET;

  /* Retrieve packets without having to know your IP address,
	 * and retrieve packets from all network interfaces if the
	 * machine has multiple ones
	 */
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Associate the address with the socket */
  if (bind(listenfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("bind");
    exit(-1);
  }

  listen(listenfd, 1);

  return (listenfd);
}

int acceptConection(int connfd) {
  int dconnfd = -1;
  sockaddr_in serverAddr, cliAddr;

  socklen_t cliLen = sizeof(cliAddr);

  /* A structure to store the client address */
  if ((dconnfd = accept(connfd, (sockaddr *)&cliAddr, &cliLen)) < 0) {
    perror("accept");
    exit(-1);
  }
}

int getFileSize(const char *fileName) {
  /* The structure containing the file information */
  struct stat fileInfo;

  /* Get the file size */
  if (stat(fileName, &fileInfo) < 0)
    return -1;

  /* Return the file size */
  return fileInfo.st_size;
}