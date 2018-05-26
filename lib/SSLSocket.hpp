#ifndef SSLSocket_hpp
#define SSLSocket_hpp

#include "Helper.hpp"
#include "Socket.hpp"
#include "SSLHandshake.hpp"
#include "Timeout.hpp"

#include "Config.hpp"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include <ssl.h>
#include <engine.h>
#include <err.h>

#include <string>
#include <iostream>
#include <mutex>


using namespace std;

class SSLSocket: public Socket
{

 public:

     SSLSocket(const string, const uint16_t);
     ~SSLSocket();

     void doClientHandshake();
     bool doServerHandshake();
     void doServerHandshakeLoop();

     void shutdown();
     void renegotiate();

     void setupSSLEngine();
     void setupSSL();
     void ProxyHandshake(const string);
     void recvDataChunk();
     bool sendDataChunk(const string);

     void resetRecvBuffer();
     void free();
     void lock();
     void unlock();

     string Received;

     bool CloseConnection;
     bool ChunkEndReached;

 private:

     mutex SendLock;

     SSL_CTX* SSLContext;
     SSL *SSLConnection;

     ENGINE* PKCS11Engine;

     EVP_PKEY* PrivkeyPointer;

     char RecvBuffer[TMP_BUFFER_SIZE];

     SSLHandshake *Handshake;

     Timeout TimeoutSendObj;

};

#endif
