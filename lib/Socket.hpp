#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <thread>

#include "Debug.cpp"
#include "Filedescriptor.hpp"

using namespace std;


class Socket
{

 public:

     Socket(const string, const uint16_t);
     ~Socket();

     virtual void setupSocket();
     virtual void setupServer();
     virtual void waitClientConnection();
     virtual void connectClient();
     virtual void recvSocketDataRaw();
     virtual int setupClientConnection();

     virtual void setFDNonblocking(int);
     virtual void setSocketFDNonblocking();
     virtual void setServerAcceptedFDNonblocking();

     static void processClient(int, Socket*);

     int SocketFD;
     int ServerAcceptedFD;

     Filedescriptor SocketFDObj;

 private:

     struct sockaddr_in SocketAddr;

     const string ListenAddress;
     const uint16_t ListenPort;

     struct sockaddr_in ClientAddr;

};

#endif
