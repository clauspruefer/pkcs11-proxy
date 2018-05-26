#ifndef SSLSocket_hpp
#define SSLSocket_hpp

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

     void setupSSLEngine();
     void setupSSL();
     void acceptServerSSL();
     void connectClientSSL();
     void ProxyHandshake(const string);
     void SSLHandshake();
     void SSLShutdown();
     void SSLRenegotiate();
     void recvDataChunk();
     void sendDataChunk(const string);
     void resetRecvBuffer();
     void free();
     void lock();
     void unlock();

     SSL *SSLConnection;

     //int RecvBytes;
     //int SendBytes;

     string Received;

     bool CloseConnection;
     bool ChunkEndReached;

 private:

     mutex SendLock;

     SSL_CTX* SSLContext;
     ENGINE* PKCS11Engine;

     EVP_PKEY* PrivkeyPointer;

     char RecvBuffer[TMP_BUFFER_SIZE];

};

#endif
