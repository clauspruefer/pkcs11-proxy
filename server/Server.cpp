#include "Server.hpp"


using namespace std;

void Server::processClient(const string ClientUUID, shared_ptr<Connection<Socket>> Connection, SSLSocket *ServerConn)
{

    DBG(40, "New thread (proxy connection) setup. ClientUUID:" << ClientUUID);

    shared_ptr<Socket> LocalConn(Connection->get(ClientUUID));

    LocalConn->setupSocket();
    LocalConn->setSocketFDNonblocking();
    LocalConn->connectClient();

    DBG(40, "Proxy connection to local socket setup succesful.");

    Timeout TimeoutObj;

    TimeoutObj.setTimeoutBlocking(false);
    TimeoutObj.setTimeout(SOCKET_IDLE_TIMEOUT_SEC, 0);

    while (TimeoutObj.checkTimeoutReached() == false) {

        DBG(200, "Enter proxy connection loop. ClientUUID:" << ClientUUID);

        LocalConn->recvSocketDataRaw();

        if (LocalConn->SocketFDObj.DataReceived) {

            DBG(200, "Received local proxy data ClientUUID:" << ClientUUID << " FDNr:" << LocalConn->SocketFDObj.MainFD << " data:" << LocalConn->SocketFDObj.Received);

            //- reset timeout
            TimeoutObj.reset();

            stringstream SendData;

            SendData << ClientUUID << LocalConn->SocketFDObj.Received;

            ServerConn->lock();
            ServerConn->sendDataChunk(SendData.str());
            ServerConn->unlock();

        }

        if (LocalConn->SocketFDObj.CloseConnection) {

            DBG(40, "Local Connection close received. Sending client close notify.");

            stringstream CloseData;

            CloseData << ClientUUID << string(PROXY_CMD_CLOSE_CONNECTION);

            ServerConn->lock();
            ServerConn->sendDataChunk(CloseData.str());
            ServerConn->unlock();

            break;
        }

        //- sleep to keep cpu time idle
        this_thread::sleep_for(chrono::milliseconds(1));

    }

    DBG(40, "Closing local connection due to endpoint close or timeout.");

    LocalConn->SocketFDObj.closeFD();

    DBG(40, "Removing connection from internal memory. Exit thread.");

    Connection->erase(ClientUUID);

}
