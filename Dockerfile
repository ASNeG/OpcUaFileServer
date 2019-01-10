FROM asneg/opcuastack:release-4.1.0

# Prepare workdir
ADD / /tmp
RUN mkdir /build
WORKDIR /build

# Build
RUN cmake /tmp/src
RUN make
RUN make install

# Start OpcUaFileServer
EXPOSE 8891 4840
CMD ["OpcUaServer3", "/etc/OpcUaStack/OpcUaFileServer/OpcUaServer.xml"]
