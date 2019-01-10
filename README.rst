OpcUaFileServer
========================================================

Installation
--------------------------------------------------------

In order to install OpcUaFileServer in your machine type the following command:

**On Linux** 

::
    $ sh build.sh -t local
	 
	
**On Windows**

::
    $ build.bat local


Docker
--------------------------------------------------------

You can use Docker to run OpcUaFileServer:

::
    $ docker build -t OpcUaFileServer:latest .
    $ docker run -d -p 8891:8891 OpcUaFileServer:latest
