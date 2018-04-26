# proxsox
proxy server in c using sockets and openssl
requests to localhost:8080 will return the response of a get request to smu.edu
this is my submission for **networks homework 6**

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

  - [quickstart](#quickstart)
  - [output](#output)
  - [how to run](#how-to-run)
      - [local build](#local-build)
      - [docker build](#docker-build)
  - [references](#references)

<!-- markdown-toc end -->


## quickstart
_assuming you have docker installed on your computer_
```
docker build -t proxsox .
docker run -it --rm --name run -p 8080:8080 proxsox
```

this will start the server listening on port 8080, then follow the onscreen
prompts for any other info

## output
**Program output while running**
![Sample program output](./sample-output.png)

**Program response page**
![Rendered response page](./response-page.png)

## how to run
since i have a windows computer, development and testing has been done using
docker, and so i've included the dockerfile necessary to run and build the program.

but if you have gcc, libssl-dev, and the ability to use a Makefile, you should
be able to build locally without an issue.

if that doesn't work then **please use the docker environment**

### local build
If you have make, then you can just use the included Makefile. Simply run `make`
and then run the executable as `./proxsox` and you should be good to go.

### docker build
If you don't want to build the application locally, then you can also run in
docker.

First, build the container with ```docker build -t proxsox .```
Then, you can run the container with ```docker run -it --rm --name run -p 8080:8080 proxsox```

## references
- [stackoverflow thread on http requests in c](https://stackoverflow.com/questions/30470505/http-request-using-sockets-in-c?rq=1)
- [stackoverflow thread on makefiles](https://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-for-gcc-on-linux)
- [the gcc docker image](https://hub.docker.com/_/gcc/)
- [compilation flags](https://stackoverflow.com/questions/6304086/how-to-compile-a-simple-program-with-openssl)
- [integrating openssl](https://stackoverflow.com/questions/16255323/make-an-https-request-using-sockets-on-linux)
- [the man pages for openssl](https://www.openssl.org/docs/man1.1.1/man3/SSL_read.html)
- [beej's guide to network programming](https://beej.us/guide/bgnet/html/multi/advanced.html#sonofdataencap)
