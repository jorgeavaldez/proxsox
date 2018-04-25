#include <sys/types.h>
#include <sys/socket.h> /* socket, connect */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */

// open ssl bc smu uses https
#include <openssl/ssl.h>

#define PORT 8080

SSL_CTX *ssl_ctx;

void createServer() {
  int serverfd, new_sock, valread;
  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);

  char buf[2056] = {0};
  char *server_res;

  // create the socket
  if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("failed creating server socket");
    exit(1);
  }

  // set options to bind to the port
  if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &opt, sizeof(opt))) {
    printf("setsockopt failed");
    exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(serverfd, (struct sockaddr *)&address,
           sizeof(address)) < 0) {
    printf("binding to port failed");
    exit(1);
  }

  if (listen(serverfd, 3) < 0) {
    printf("listen failed");
    exit(1);
  }

  if ((new_sock = accept(serverfd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen)) < 0) {
    printf("accept failed");
    exit(1);
  }

  valread = recv(new_sock, buf, 2056, 0);
  printf("%s\n", buf);
}

void makeSSLRequest () {
  struct addrinfo hints, *res;
  int sockfd;

  char buf[2056];
  int byte_count;

  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_INET;
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
    printf("connected");
  }

  // use the ssl functions instead of the defaults to read
  char *header = "GET / HTTP/1.1\r\nHost: www.smu.edu\r\n\r\n";
  SSL_write(conn, header, strlen(header));

  printf("Request to www.smu.edu sent...\n");

  // receive data
  byte_count = SSL_read(conn, buf, sizeof(buf));
  printf("recv()'d %d bytes of data in buf\n", byte_count);
  printf("%.*s", byte_count, buf);
}

int main(void) {
  // ssl_ctx needs to be a global var
  ssl_ctx = SSL_CTX_new(SSLv23_client_method());

  //makeSSLRequest();
  createServer();
  return 0;
}
