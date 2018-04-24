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
