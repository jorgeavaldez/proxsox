FROM gcc

COPY . /usr/src/proxsox
WORKDIR /usr/src/proxsox
RUN apt-get update
RUN apt-get upgrade -y
RUN make clean
RUN make

CMD ["./proxsox"]