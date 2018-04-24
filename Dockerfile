FROM gcc

COPY . /usr/src/proxsox
WORKDIR /usr/src/proxsox
RUN make

CMD ["./proxsox"]