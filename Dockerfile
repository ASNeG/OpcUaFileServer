FROM asneg/opcuastack:release-4.0.0

# Prepare workdir
ADD / /tmp
RUN mkdir /build
WORKDIR /build

# Build
RUN cmake /tmp/src
RUN make
RUN make install

# Start OpcUaFileServer
EXPOSE 4840 4840
CMD ["OpcUaServer4", "/etc/OpcUaStack/OpcUaFileServer/OpcUaServer.xml"]
