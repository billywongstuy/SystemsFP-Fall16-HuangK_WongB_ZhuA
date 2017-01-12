#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int server_setup() {
  
  int sd;

  sd = socket( AF_INET, SOCK_STREAM, 0 );

  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(9001);
  bind( sd, (struct sockaddr *)&sock, sizeof(sock) );

  return sd;
}

int server_connect(int sd) {
  int connection;

  listen(sd, 1);

  struct sockaddr_in sock1;
  unsigned int sock1_len = sizeof(sock1);
  connection = accept( sd, (struct sockaddr *)&sock1, &sock1_len );

  printf("[server] connected to %s\n", inet_ntoa( sock1.sin_addr ) );
  
  return connection;
}


int client_connect( char *host ) {
  int sd;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );

  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  inet_aton( host, &(sock.sin_addr));
  sock.sin_port = htons(9001);

  bind( sd, (struct sockaddr *)&sock, sizeof(sock) );

  printf("[client] connecting to: %s\n", host );
  connect( sd, (struct sockaddr *)&sock, sizeof(sock) );

  return sd;
}
