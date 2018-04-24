FROM gcc

COPY . /usr/src/proxsox
WORKDIR /usr/src/proxsox
RUN make clean
RUN make

CMD ["./proxsox"]