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

void makeSSLRequest (char* server_res) {
  struct addrinfo hints, *res;
  int sockfd;

  int byte_count, total_byte_count;

  char buf[101] = {0};

  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.smu.edu", "443", &hints, &res);

  sockfd = socket(
                  res->ai_family,
                  res->ai_socktype,
                  res->ai_protocol);

  printf("Establishing socket connection to https://www.smu.edu...\n");

  connect(
          sockfd,
          res->ai_addr,
          res->ai_addrlen);

  // set up openssl
  SSL_library_init();
  SSL_load_error_strings();

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
    printf("Established ssl connection with https://smu.edu\n");
    printf("Ready to send get request\n\n");
  }

  // The get request for smu
  char* usr_header = "GET / HTTP/1.1\r\nHost: www.smu.edu\r\n\r\n";

  // use the ssl functions instead of the defaults to read
  printf("Sending request to https://www.smu.edu\n\n%s", usr_header);
  SSL_write(conn, usr_header, strlen(usr_header));

  printf("Request to https://www.smu.edu sent...\n");

  // receive data and write it to the server_res
  byte_count = SSL_read(conn, buf, 100);
  total_byte_count = byte_count;

  // copy the buffer into the ultimate response
  strcpy(server_res, buf);

  // printf("recv()'d %d bytes of data in buf\n", byte_count);
  printf("\nReading response from https://www.smu.edu\n");
  printf("Constructing response buffer...\n\n");

  int expected_byte_count = SSL_peek(conn, buf, 100);
  while (expected_byte_count > 15) {
    byte_count = SSL_read(conn, buf, 100);
    //printf("\nbyte_count: %d\n", byte_count);
    err = SSL_get_error(conn, byte_count);

    switch(err){
      case(SSL_ERROR_NONE):
        total_byte_count = total_byte_count + byte_count;
        //printf("%.*s\n", byte_count, buf);
        strcat(server_res, buf);
        //printf("recv()'d %d bytes of data in buf\n", total_byte_count);

        break;

    case(SSL_ERROR_ZERO_RETURN):
        printf("ZERO RETURN");
        break;

      case(SSL_ERROR_WANT_READ):
        printf("WANT_READ");
        break;

      case(SSL_ERROR_WANT_CONNECT):
        printf("WANT CONNECT");
        break;

      default:
        printf("default? \n");
        break;
    }

    expected_byte_count = SSL_peek(conn, buf, 100);
  }

  byte_count = SSL_read(conn, buf, SSL_peek(conn, buf, expected_byte_count));
  total_byte_count = total_byte_count + byte_count;
  //printf("%.*s\n", byte_count, buf);
  printf("Received %d total bytes of data from https://www.smu.edu\n\n", total_byte_count);

  strcat(server_res, buf);
  //printf("\n\nULTIMATERESPONSE\n\n%.*s\n", total_byte_count, server_res);
}

void createServer() {
  int serverfd, new_sock, valread;
  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);

  char buf[2056] = {0};
  // strnlen doesn't help like it should, i just hardcoded the length
  char server_res[50888] = {0};

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

  printf("Server listening at port %d\n", PORT);
  if (listen(serverfd, 3) < 0) {
    printf("listen failed");
    exit(1);
  }

  if ((new_sock = accept(serverfd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen)) < 0) {
    printf("accept failed");
    exit(1);
  }

  printf("Server received request...\n");

  valread = recv(new_sock, buf, sizeof(buf), 0);
  //printf("Server recv()'d %d bytes of data in buf\n", valread);
  printf("Server received the following request from the client: %.*s\n\n", valread, buf);

  // now use the value in buf as the header for the next request
  makeSSLRequest(server_res);
  //printf("Request to remote received: %.*s\n", (int)sizeof(server_res), server_res);
  // printf("Request to remote received: %s\n", server_res);

  printf("Server ready to respond...\n\n");

  // send server_res back to the user
  valread = send(new_sock, server_res, strlen(server_res), 0);
  printf("Server responded yay!");
}

int main(void) {
  // ssl_ctx needs to be a global var
  ssl_ctx = SSL_CTX_new(SSLv23_client_method());

  //makeSSLRequest();
  createServer();
  return 0;
}
