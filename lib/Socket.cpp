#include "Socket.hpp"

using namespace std;

Socket::Socket(const string Address, const uint16_t Port) :
    ServerAcceptedFD(0),
    SocketFDObj(Filedescriptor()),
    ListenAddress(Address),
    ListenPort(Port)
{
    DBG(200, "Socket Constructor called.");
}

Socket::~Socket()
{
    DBG(200, "Socket Destructor called.");
}

void Socket::setupSocket()
{

    DBG(10, "Socket setupSocket() called. Host:" << ListenAddress << " Port:" << ListenPort);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);

    memset((char*)&SocketAddr, 0, sizeof(SocketAddr));

    SocketAddr.sin_family = AF_INET;
    SocketAddr.sin_port = htons(ListenPort);
    SocketAddr.sin_addr.s_addr = inet_addr(ListenAddress.c_str());

    //- set socket reuse address flag
    int flags = fcntl(SocketFD, F_GETFL, 0);

    if (fcntl(SocketFD, F_SETFL, flags | SO_REUSEADDR) < 0) {
        ERR("Error setting Socket flag SO_REUSEADDR.");
        exit(EXIT_FAILURE);
    }

    //- set filedescriptor object
    SocketFDObj.setFD(SocketFD);

}

void Socket::setSocketFDNonblocking() {
    setFDNonblocking(SocketFD);
}

void Socket::setServerAcceptedFDNonblocking() {
    setFDNonblocking(ServerAcceptedFD);
}

void Socket::setFDNonblocking(int fd) {

    int flags = fcntl(fd, F_GETFL, 0);

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK ) < 0) {
        ERR("Error setting Socket flag O_NONBLOCKING.");
        exit(EXIT_FAILURE);
    }

}

void Socket::setupServer()
{

    int rc = bind(SocketFD, (struct sockaddr*)&SocketAddr, sizeof(SocketAddr));

    if (rc != 0) {
        ERR("Error binding Socket.");
        exit(EXIT_FAILURE);
    }

    listen(SocketFD, 0);

}

void Socket::waitClientConnection()
{
    while (this->setupClientConnection() <= 0) {
        this_thread::sleep_for(chrono::milliseconds(SERVER_WAIT_CONNECTION_SLEEP_INTERVAL_MSEC));
    }
}

int Socket::setupClientConnection()
{
    DBG(200, "Socket setupClientConnection() called.");

    int ClientFD = 0;
    int ClientAddrLen = sizeof(ClientAddr);

    ClientFD = accept(SocketFD, (struct sockaddr*)&ClientAddr, (socklen_t*)&ClientAddrLen);

    if (ClientFD > 0) {
        DBG(200, "Socket setupClientConnection() accepted ClientFD:" << ClientFD);
    }

    if (ClientFD == -1) {
        DBG(200, "Accept Error:" << strerror(errno));
    }

    //- set fd for server accept
    ServerAcceptedFD = ClientFD;

    return ClientFD;
}

void Socket::connectClient()
{
    DBG(200, "Socket connectClient() called.");

    bool connected = false;
    while (connected == false) {

        int rc = connect(SocketFD, (struct sockaddr *)&SocketAddr, sizeof(SocketAddr));

        if (rc == 0) {
            connected = true;
        }

        if (rc == -1) {
            DBG(10, "Connect Error:" << strerror(errno));
            sleep(1);
        }
    }
}

void Socket::recvSocketDataRaw()
{
    SocketFDObj.recvDataRaw();
}
