#include <sys/types.h>
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */

void makeRequest() {
  struct addrinfo hints, *res;
  int sockfd;

  char buf[2056];
  int byte_count;

  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("http://www.smu.edu", "80", &hints, &res);

  sockfd = socket(
                  res->ai_family,
                  res->ai_socktype,
                  res->ai_protocol);

  printf("Connecting...\n");

  connect(
          sockfd,
          res->ai_addr,
          res->ai_addrlen);

  printf("Connected!\n");

  char *header = "GET / HTTP/1.1\r\nHost: www.smu.edu\r\n\r\n";
  send(sockfd, header, strlen(header), 0);

  printf("Request to www.smu.edu sent...\n");

  // receive data
  byte_count = recv(sockfd, buf, sizeof(buf), 0);
  printf("recv()'d %d bytes of data in buf\n", byte_count);
  printf("%.*s", byte_count, buf);
}

int main(void) {
  printf("Let's go!!!\n");
  makeRequest();
  return 0;
}
