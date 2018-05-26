#include <errno.h>

#include "Helper.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "SSLSocket.hpp"


using namespace std;

SSLSocket::SSLSocket(const string Address, const uint16_t ListenPort) :
    Socket(Address, ListenPort),
    CloseConnection(false),
    ChunkEndReached(false),
    PrivkeyPointer(NULL)
{
    cout << "SSLSocket Constructor called." << endl;
    resetRecvBuffer();
}

SSLSocket::~SSLSocket()
{
    cout << "SSLSocket Destructor called." << endl;
}

void SSLSocket::setupSSLEngine()
{

    const string EngineID = "dynamic";

    ENGINE_load_builtin_engines();

    PKCS11Engine = ENGINE_by_id(EngineID.c_str());

    //- if engine is not available, exit
    if (!PKCS11Engine) {
        cout << "Error setting up Openssl pkcs11 Engine!" << endl;
        exit(EXIT_FAILURE);
    }

    ENGINE_load_dynamic();

    ENGINE_ctrl_cmd_string(PKCS11Engine, "SO_PATH", "/usr/local/lib/engines/engine_pkcs11.so", 0);
    ENGINE_ctrl_cmd_string(PKCS11Engine, "ID", "pkcs11", 0);
    ENGINE_ctrl_cmd_string(PKCS11Engine, "LIST_ADD", "1", 0);
    ENGINE_ctrl_cmd_string(PKCS11Engine, "LOAD", NULL, 0);
    ENGINE_ctrl_cmd_string(PKCS11Engine, "MODULE_PATH", "/usr/local/lib/opensc-pkcs11.so", 0);

    int rc = ENGINE_init(PKCS11Engine);
    cout << "Engine init result:" << rc << endl;

    const char* LoadedEngineID = ENGINE_get_id(PKCS11Engine);
    cout << "Loaded engine with id:" << LoadedEngineID << endl;

    ENGINE_set_default(PKCS11Engine, ENGINE_METHOD_RSA | ENGINE_METHOD_ECDSA | ENGINE_METHOD_ECDH);

    const char *Prompt = "Enter SmCloseConnection = false;artcard User PIN:";

    string UserPin(getpass(Prompt));

    if (UserPin.length() > 20) { exit(EXIT_FAILURE); }

    string ObjLocation;
    ObjLocation.append(Config::CardSlot);
    ObjLocation.append(string(":"));
    ObjLocation.append(Config::ContainerId);

    ENGINE_ctrl_cmd_string(PKCS11Engine, "PIN", UserPin.c_str(), 0);
    PrivkeyPointer = ENGINE_load_private_key(PKCS11Engine, ObjLocation.c_str(), NULL, NULL);

    UserPin.clear();

}

void SSLSocket::setupSSL()
{

    DBG(10, "SSLSocket::setupSSL() called.");

    //- setup ssl engine if no key file given
    if (Config::KeyFile.length() == 0) {
        setupSSLEngine();
    }

    //- init ssl base settings
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();

    //- setup client SSL context
    if (Config::Type == "client") {
        SSLContext = SSL_CTX_new(TLSv1_2_client_method());
    }

    //- setup server SSL context
    if (Config::Type == "server") {
        //- set server method
        SSLContext = SSL_CTX_new(TLSv1_2_server_method());
        //- enable client cert verification
        SSL_CTX_set_verify(SSLContext, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);

    }

    //- add ciphers
    int Ciphers = SSL_CTX_set_cipher_list(SSLContext, "HIGH");
    cout << "SSL setup ciphers rc:" << Ciphers << endl;

    //- disable compression
    SSL_CTX_set_options(SSLContext, SSL_OP_NO_COMPRESSION);

    //- disable ssl tickets
    SSL_CTX_set_options(SSLContext, SSL_OP_NO_TICKET);

    //- disable dtls
    SSL_CTX_set_options(SSLContext, SSL_OP_NO_DTLSv1);
    SSL_CTX_set_options(SSLContext, SSL_OP_NO_DTLSv1_2);

    if (Config::Type == "server") {

        //- load CA cert
        if (SSL_CTX_load_verify_locations(SSLContext, Config::CACertFile.c_str(), NULL) <= 0) {
            cout << "SSL load verify locations error." << endl;
            exit(EXIT_FAILURE);
        }

        SSL_CTX_set_client_CA_list(SSLContext, SSL_load_client_CA_file(Config::CACertFile.c_str()));

    }

    //- load cert
    if (SSL_CTX_use_certificate_file(SSLContext, Config::CertFile.c_str(), SSL_FILETYPE_PEM) <=0) {
        cout << "SSL load cert file error." << endl;
        exit(EXIT_FAILURE);
    }

    //- load private key file
    if (Config::KeyFile.length() > 0 && SSL_CTX_use_PrivateKey_file(SSLContext, Config::KeyFile.c_str(), SSL_FILETYPE_PEM) <=0) {
        cout << "SSL load private key file error." << endl;
        exit(EXIT_FAILURE);
    }

    if (Config::KeyFile.length() == 0 && PrivkeyPointer == NULL) {
        cout << "No key file configuration found and no valid key from smartcard." << endl;
        exit(EXIT_FAILURE);
    }

    //- use private key (pointer) from pkcs11 engine
    if (Config::KeyFile.length() == 0 && SSL_CTX_use_PrivateKey(SSLContext, PrivkeyPointer) <= 0) {
        cout << "SSL load smartcard private key error." << endl;
        exit(EXIT_FAILURE);
    }

}

void SSLSocket::acceptServerSSL()
{

    DBG(100, "SSLSocket::acceptServerSSL().");

    SSLConnection = SSL_new(SSLContext);
    SSL_set_fd(SSLConnection, ServerAcceptedFD);

    bool completedHandshake = false;
    while (completedHandshake == false) {

        DBG(200, "Handshake loop.");

        int rc = SSL_accept(SSLConnection);
        int result = SSL_get_error(SSLConnection, rc);

        DBG(200, "Result:" << result << " errror:" << strerror(errno));

        switch(result) {

            case SSL_ERROR_NONE :
                DBG(40, "SSL handshake (accept) completed.");
                completedHandshake = true;

            case SSL_ERROR_ZERO_RETURN :
                DBG(40, "SSL accept aborted.");

            case SSL_ERROR_WANT_ACCEPT :
                DBG(40, "SSL accept continue.");

        }
    }
}

void SSLSocket::SSLHandshake()
{

    DBG(100, "SSLSocket::SSLHandshake().");

    bool completedHandshake = false;
    while (completedHandshake == false) {

        DBG(200, "Handshake loop.");

        int rc = SSL_do_handshake(SSLConnection);
        int result = SSL_get_error(SSLConnection, rc);

        DBG(200, "Result:" << result << " errror:" << strerror(errno));

        switch(result) {

            case SSL_ERROR_NONE :
                DBG(40, "SSL handshake (connect) completed.");
                completedHandshake = true;

            case SSL_ERROR_ZERO_RETURN :
                DBG(40, "SSL connect aborted.");

            case SSL_ERROR_WANT_CONNECT :
                DBG(40, "SSL connect continue.");

        }

    }
}

void SSLSocket::SSLShutdown()
{

    DBG(100, "SSLSocket::SSLShutdown().");

    bool HandshakeCompleted = false;
    while (HandshakeCompleted == false) {

        DBG(200, "Handshake loop.");

        int rc = SSL_shutdown(SSLConnection);
        int result = SSL_get_error(SSLConnection, rc);

        DBG(200, "Result:" << result << " errror:" << strerror(errno));

        switch(result) {

            case SSL_ERROR_NONE :
                DBG(40, "SSL handshake (connect) completed.");
                HandshakeCompleted = true;

            case SSL_ERROR_ZERO_RETURN :
                DBG(40, "SSL connect aborted.");

            case SSL_ERROR_WANT_CONNECT :
                DBG(40, "SSL connect continue.");

        }

    }

}

void SSLSocket::SSLRenegotiate()
{

    DBG(100, "SSLSocket::SSLRenegotiate().");

    bool HandshakeCompleted = false;
    while (HandshakeCompleted == false) {

        DBG(200, "Handshake loop.");

        int rc = SSL_renegotiate(SSLConnection);
        int result = SSL_get_error(SSLConnection, rc);

        DBG(200, "Result:" << result << " errror:" << strerror(errno));

        switch(result) {

            case SSL_ERROR_NONE :
                DBG(40, "SSL handshake (connect) completed.");
                HandshakeCompleted = true;

            case SSL_ERROR_ZERO_RETURN :
                DBG(40, "SSL connect aborted.");

            case SSL_ERROR_WANT_CONNECT :
                DBG(40, "SSL connect continue.");

        }

    }

}

void SSLSocket::connectClientSSL()
{

    DBG(100, "SSLSocket::connectClientSSL().");

    SSLConnection = SSL_new(SSLContext);
    SSL_set_fd(SSLConnection, SocketFD);

    bool HandshakeCompleted = false;
    while (HandshakeCompleted == false) {

        DBG(200, "Handshake loop.");

        int rc = SSL_connect(SSLConnection);
        int result = SSL_get_error(SSLConnection, rc);

        DBG(200, "Result:" << result << " errror:" << strerror(errno));

        switch(result) {

            case SSL_ERROR_NONE :
                DBG(40, "SSL handshake (connect) completed.");
                HandshakeCompleted = true;

            case SSL_ERROR_ZERO_RETURN :
                DBG(40, "SSL connect aborted.");

            case SSL_ERROR_WANT_CONNECT :
                DBG(40, "SSL connect continue.");

        }

    }
}

void SSLSocket::ProxyHandshake(string ClientID) {
    stringstream SendData;
    SendData << ClientID << string(PROXY_CMD_NEW_CONNECTION);
    sendDataChunk(SendData.str());
}

void SSLSocket::sendDataChunk(const string Data)
{

    DBG(200, "SSLSocket::sendDataChunk().");
    DBG(150, "Input Data:" << Data);

    string SendData;

    SendData.append(string(PAYLOAD_MARKER_PREFIX));
    SendData.append(Data);
    SendData.append(string(PAYLOAD_MARKER_POSTFIX));

    while (true) {

        int SendBytes = SSL_write(SSLConnection, SendData.c_str(), SendData.length());
        int SendResult = SSL_get_error(SSLConnection, SendBytes);

	DBG(150, "Bytes:" << SendBytes << " ResultCode:" << SendResult << " ResultError:" << strerror(SendResult));
	if (SendResult == 0) { break; }
    }

    DBG(150, "Sent Data:" << SendData);

}

void SSLSocket::recvDataChunk()
{

    DBG(200, "recvDataChunkSSL()");

    CloseConnection = false;

    int RecvBytes = -1;
    RecvBytes = SSL_read(SSLConnection, RecvBuffer, TMP_BUFFER_SIZE);

    if (RecvBytes == 0) {
        DBG(10, "SSL connection close initiated.");
        CloseConnection = true;
        return;
    }

    int result = SSL_get_error(SSLConnection, RecvBytes);

    if (result == SSL_ERROR_WANT_READ) {
        DBG(200, "SSL error want read.");
        return;
    }

    if (result == SSL_ERROR_SYSCALL || result == SSL_ERROR_SSL) {
        DBG(40, "SSLSocket.recvDataChunk() SSL_ERROR_SYSCALL or SSL_ERROR_SSL error:" << strerror(errno));
    }

    if (RecvBytes > 0) {
        DBG(200, "recvDataChunkSSL() RecvBytes:" << RecvBytes);

        Received.append(RecvBuffer, RecvBytes);

        string CompareStart(PAYLOAD_MARKER_PREFIX);
        string CompareEnd(PAYLOAD_MARKER_POSTFIX);

        if (Received.compare(0, CompareStart.size(), CompareStart) == 0 && Received.compare(Received.size()-CompareEnd.size(), CompareEnd.size(), CompareEnd) == 0) {
            Received.replace(Received.begin(), Received.begin()+CompareStart.size(), "");
            Received.replace(Received.end()-CompareEnd.size(), Received.end(), "");
            ChunkEndReached = true;
            resetRecvBuffer();
        }

    }

}

void SSLSocket::resetRecvBuffer()
{
    memset(RecvBuffer, 0, TMP_BUFFER_SIZE);
}

void SSLSocket::free()
{
    SSL_free(SSLConnection);
}

void SSLSocket::lock()
{
    SendLock.lock();
}

void SSLSocket::unlock()
{
    SendLock.unlock();
}
