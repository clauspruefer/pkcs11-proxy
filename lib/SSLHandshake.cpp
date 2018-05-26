#include "SSLHandshake.hpp"


using namespace std;

SSLHandshake::SSLHandshake()
{
    DBG(200, "SSLHandshake Constructor called.");
}

SSLHandshake::~SSLHandshake()
{
    DBG(200, "SSLHandshake Destructor called.");
}

void SSLHandshake::reset()
{

    acceptStatus = 0;
    connectStatus = 0;
    HandshakeStatus = 0;
    shutdownStatus = 0;
    renegotiateStatus = 0;

    acceptCount = 0;
    connectCount = 0;
    HandshakeCount = 0;
    shutdownCount = 0;
    renegotiateCount = 0;

}

void SSLHandshake::accept(SSL *c)
{

    int rc = SSL_accept(c);
    acceptStatus = SSL_get_error(c, rc);

    DBG(40, "Accept Status:" << acceptStatus << " errror:" << strerror(errno));

}

void SSLHandshake::connect(SSL *c)
{

    int rc = SSL_connect(c);
    connectStatus = SSL_get_error(c, rc);

    DBG(40, "Connect Status:" << connectStatus << " errror:" << strerror(errno));

}

void SSLHandshake::Handshake(SSL *c)
{

    int rc = SSL_do_handshake(c);
    HandshakeStatus = SSL_get_error(c, rc);

    DBG(40, "Handshake Status:" << HandshakeStatus << " errror:" << strerror(errno));

}

void SSLHandshake::shutdown(SSL *c)
{

    int rc = SSL_shutdown(c);
    shutdownStatus = SSL_get_error(c, rc);

    DBG(40, "Shutdown Status:" << shutdownStatus << " errror:" << strerror(errno));

}

void SSLHandshake::renegotiate(SSL *c)
{

    int rc = SSL_renegotiate(c);
    renegotiateStatus = SSL_get_error(c, rc);

    DBG(40, "Renegotiation Status:" << renegotiateStatus << " errror:" << strerror(errno));

}

bool SSLHandshake::acceptLoop(SSL *c)
{

    while (acceptCount < SSL_HANDSHAKE_RETRY_COUNT) {
        this->accept(c);
        DBG(100, "AcceptStatus:" << acceptStatus << "compare:" << SSL_ERROR_NONE);
        if (acceptStatus == SSL_ERROR_NONE) {
            return true;
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(SSL_HANDSHAKE_RETRY_SLEEP_INTERVAL_MSEC));
        }
        acceptCount++;
    }

    return false;
}

bool SSLHandshake::connectLoop(SSL *c)
{

    while (connectCount < SSL_HANDSHAKE_RETRY_COUNT) {
        this->connect(c);
        if (connectStatus == SSL_ERROR_NONE) {
            return true;
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(SSL_HANDSHAKE_RETRY_SLEEP_INTERVAL_MSEC));
        }
        connectCount++;
    }

    return false;
}

bool SSLHandshake::HandshakeLoop(SSL *c)
{

    while (HandshakeCount < SSL_HANDSHAKE_RETRY_COUNT) {
        this->Handshake(c);
        if (HandshakeStatus == SSL_ERROR_NONE) {
            return true;
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(SSL_HANDSHAKE_RETRY_SLEEP_INTERVAL_MSEC));
        }
        HandshakeCount++;
    }

    return false;
}

bool SSLHandshake::renegotiateLoop(SSL *c)
{

    while (renegotiateCount < SSL_HANDSHAKE_RETRY_COUNT) {
        this->renegotiate(c);
        if (renegotiateStatus == SSL_ERROR_NONE) {
            return true;
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(SSL_HANDSHAKE_RETRY_SLEEP_INTERVAL_MSEC));
        }
        renegotiateCount++;
    }

    return false;
}
