#ifndef SSLHandshake_hpp
#define SSLHandshake_hpp

#include <ssl.h>

using namespace std;

class SSLHandshake
{

 public:

     SSLHandshake();
     ~SSLHandshake();

     void reset();

     void accept(SSL*);
     void connect(SSL*);
     void Handshake(SSL*);
     void shutdown(SSL*);
     void renegotiate(SSL*);

     bool acceptLoop(SSL*);
     bool connectLoop(SSL*);
     bool HandshakeLoop(SSL*);
     bool shutdownLoop(SSL*);
     bool renegotiateLoop(SSL*);

 private:

     int acceptStatus;
     int connectStatus;
     int HandshakeStatus;
     int shutdownStatus;
     int renegotiateStatus;

     int acceptCount;
     int connectCount;
     int HandshakeCount;
     int shutdownCount;
     int renegotiateCount;

};

#endif
