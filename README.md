# proxsox
socket program for networks hw 6

## building
### local build
If you have make, then you can just use the included Makefile. Simply run `make`
and then run the executable as `./proxsox` and you should be good to go.

### docker build
If you don't want to build the application locally, then you can also run in
docker.

First, build the container with ```docker build -t proxsox .```
Then, you can run the container with ```docker run -it --rm --name run-proxsox proxsox```

## references
- [stackoverflow thread on http requests in c](https://stackoverflow.com/questions/30470505/http-request-using-sockets-in-c?rq=1)
- [stackoverflow thread on makefiles](https://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-for-gcc-on-linux)
- [the gcc docker image](https://hub.docker.com/_/gcc/)
- [compilation flags](https://stackoverflow.com/questions/6304086/how-to-compile-a-simple-program-with-openssl)
- [integrating openssl](https://stackoverflow.com/questions/16255323/make-an-https-request-using-sockets-on-linux)

