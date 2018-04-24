#include <sys/types.h>
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */

// open ssl bc smu uses https
#include <openssl/ssl.h>

void makeSSLRequest () {
  struct addrinfo hints, *res;
  int sockfd;

  char buf[2056];
  int byte_count;

  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.smu.edu", "443", &hints, &res);

  sockfd = socket(
                  res->ai_family,
                  res->ai_socktype,
                  res->ai_protocol);

  printf("Connecting...\n");

  connect(
          sockfd,
          res->ai_addr,
          res->ai_addrlen);

  // set up openssl
  SSL_load_error_strings();
  SSL_library_init();
  // ssl_ctx needs to be a global var
  SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());

  // create an ssl connection and attach it to the socket
  SSL *conn = SSL_new(ssl_ctx);
  SSL_set_fd(conn, sockfd);

  // perform the handshake
  int err = SSL_connect(conn);
  if (err != 1) {
    printf("Handshake failed, value of %d", err);
    exit(1);
  }

  else {
    printf("Handshake succeeded my dude");
  }
}

void makeRequest() {
  struct addrinfo hints, *res;
  int sockfd;

  char buf[2056];
  int byte_count;

  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.smu.edu", "80", &hints, &res);

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
  makeSSLRequest();
  return 0;
}
