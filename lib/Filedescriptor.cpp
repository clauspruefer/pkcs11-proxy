#include "Filedescriptor.hpp"

using namespace std;

Filedescriptor::Filedescriptor():
    MainFD(0),
    closed(false)
{
    DBG(200, "Filedescriptor Constructor called.");
}

Filedescriptor::~Filedescriptor()
{
    DBG(200, "Filedescriptor Destructor called.");
}

void Filedescriptor::setFD(int FD)
{
    MainFD = FD;
    closed = false;
    setupPoll();
}

void Filedescriptor::setupPoll() {
    PollRecvfds[0].fd = MainFD;
    PollRecvfds[0].events = POLLIN;
    PollSendfds[0].fd = MainFD;
    PollSendfds[0].events = POLLOUT;
}

bool Filedescriptor::checkFDreadable() {

    int rc = poll(PollRecvfds, 1, 100);

    if (rc == 0) {
        return false;
    }

    if (rc == -1) {
        return false;
    }

    if (rc > 0 && PollRecvfds[0].revents & POLLIN) {
        return true;
    }

    return false;
}

bool Filedescriptor::checkFDwriteable() {

    int rc = poll(PollSendfds, 1, 100);

    if (rc == 0) {
        return false;
    }

    if (rc == -1) {
        return false;
    }

    if (rc > 0 && PollSendfds[0].revents & POLLOUT) {
        return true;
    }

    return false;
}

int Filedescriptor::recvData()
{
    RecvBytes = recv(MainFD, RecvBuffer, TMP_BUFFER_SIZE, 0);
    return RecvBytes;
}

void Filedescriptor::sendData(const string Data)
{
    if (closed) { return; }

    while (checkFDwriteable() == false) {
        DBG(200, "File Descriptor not writeable");
    }

    SendBytes = send(MainFD, Data.c_str(), Data.length(), 0);

}

void Filedescriptor::recvDataRaw()
{

    DBG(200, "recvDataRaw()");

    memset(RecvBuffer, 0, TMP_BUFFER_SIZE);
    Received.clear();

    DataReceived = false;
    CloseConnection = false;

    if (checkFDreadable() == false) {
        return;
    }

    RecvBytes = recv(MainFD, RecvBuffer, TMP_BUFFER_SIZE, 0);

    if (RecvBytes > 0) {
        DataReceived = true;
        Received.append(RecvBuffer, RecvBytes);
    }

    if (RecvBytes == 0) {
        CloseConnection = true;
        return;
    }

    if (RecvBytes < 0) {
        ERR("recvDataRaw() Error received:" << RecvBytes);
        exit(EXIT_FAILURE);
    }

}

void Filedescriptor::closeFD()
{
    ::close(MainFD);
    closed = true;
}
