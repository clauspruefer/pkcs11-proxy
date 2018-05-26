#ifndef Filedesc_hpp
#define Filedesc_hpp

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <ssl.h>

#include <string>
#include <iostream>
#include <sstream>

#include "Constant.hpp"

using namespace std;

class Filedescriptor
{

 public:

     Filedescriptor();
     ~Filedescriptor();

     void setFD(int);
     int recvData();
     void sendData(const string);
     void setupPoll();
     bool checkFDreadable();
     bool checkFDwriteable();
     void recvDataChunk();
     void recvDataRaw();
     void closeFD();

     int RecvBytes;
     int SendBytes;

     string Received;

     bool CloseConnection;
     bool DataReceived;

     int MainFD;

 private:

     char RecvBuffer[TMP_BUFFER_SIZE];

     struct pollfd PollRecvfds[1];
     struct pollfd PollSendfds[1];

     bool closed;
};

#endif
