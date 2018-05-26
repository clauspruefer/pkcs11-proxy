#include "Server.hpp"

#include "../lib/Config.cpp"
#include "../lib/Filedescriptor.cpp"
#include "../lib/Socket.cpp"
#include "../lib/SSLSocket.cpp"
#include "../lib/Connection.cpp"
#include "../lib/Timeout.cpp"


using namespace std;

int main()
{

    cout << "### >> ---------------------------------------------------------------" << endl;
    cout << "### >> TLS PKCS11 Proxy (Server component)" << endl;
    cout << "### >> ---------------------------------------------------------------" << endl;

    //- process xml configuration
    Config::setup();

    //- setup server socket/ssl socket
    SSLSocket ServerConn(Config::ListenHost, Config::ListenPort);
    ServerConn.setupSSL();
    ServerConn.setupSocket();
    ServerConn.setupServer();
    ServerConn.setSocketFDNonblocking();

    //- wait for client ssl connection
    DBG(10, "Waiting for SSL client connection.");

    ServerConn.waitClientConnection();
    ServerConn.setServerAcceptedFDNonblocking();
    ServerConn.acceptServerSSL();
    ServerConn.SSLHandshake();

    DBG(10, "SSL handshake completed.");

    ServerConn.resetRecvBuffer();
    ServerConn.ChunkEndReached = false;


    //- ---------------------------------------------------------------------------
    //- setup timeouts
    //- ---------------------------------------------------------------------------

    Timeout TimeoutObj;

    TimeoutObj.setTimeoutBlocking(false);
    TimeoutObj.setTimeout(SERVER_IDLE_INTERVAL_SEC, 0);


    //- ---------------------------------------------------------------------------
    //- setup client connections
    //- ---------------------------------------------------------------------------

    shared_ptr<Connection<Socket>> ClientConnections(new Connection<Socket>);


    //- ---------------------------------------------------------------------------
    //- loop until sigint received (main loop)
    //- ---------------------------------------------------------------------------

    bool RunServer = true;
    while (RunServer) {

        DBG(200, "Enter server loop. Check data chunk was received successfully.");

        ServerConn.recvDataChunk();

        if (ServerConn.ChunkEndReached) {

            //- reset server idle timeout
            TimeoutObj.reset();

            //- process received data
            string ServerData = ServerConn.Received;

            //- get client filedescriptor integer
            string ClientUUID = ServerData.substr(0,UUID_SIZE);

            //- remove first two bytes from send data
            ServerData.erase(0,UUID_SIZE);

            if (ServerData.compare(PROXY_CMD_NEW_CONNECTION) == 0) {

                DBG(40, "Handshake for new proxy connection received. ClientUUID:" << ClientUUID);

                //- setup local listenting socket/pointer
                unique_ptr<Socket> Connection(new Socket(Config::ProxyHost, Config::ProxyPort));

                //- add socket ref to map
                ClientConnections->add(ClientUUID, move(Connection));

                //- fire new thread
                thread *t = new thread(&Server::processClient, ClientUUID, ClientConnections, &ServerConn);

                //- detach thread
                t->detach();

            }

            else if (ServerData.compare(PROXY_CMD_KEEPALIVE) == 0) {
                DBG(40, "Client keepalive received. Dummy ClientUUID:" << ClientUUID);
            }

            else if (ServerData.compare(PROXY_CMD_CLOSE_CONNECTION) == 0) {
                DBG(40, "Client close connection received. ClientUUID:" << ClientUUID);

                //- close connection, remove from connections
                shared_ptr<Socket> SocketObj(ClientConnections->get(ClientUUID));
                if (SocketObj > 0) {
                    SocketObj->SocketFDObj.closeFD();
                    ClientConnections->erase(ClientUUID);
                }

            }

            else {

                DBG(40, "Client data for existant client connection received. ClientUUID:" << ClientUUID);

                shared_ptr<Socket> SocketObj(ClientConnections->get(ClientUUID));
                if (SocketObj > 0) {
                    SocketObj->SocketFDObj.sendData(ServerData);
                    DBG(200, "Sent data to local connection with ClientUUID:" << ClientUUID << " FDNr:" << SocketObj->SocketFDObj.MainFD);
                }

            }

            ServerConn.ChunkEndReached = false;
            ServerConn.Received.clear();

        }

        //- if client close connection received
        if (ServerConn.CloseConnection) {

            DBG(40, "Server close initiated. Waiting for new connection.");

            //- reset client connection map
            ClientConnections->clear();

            //- wait for new ssl client connection
            ServerConn.waitClientConnection();
            ServerConn.setServerAcceptedFDNonblocking();
            ServerConn.acceptServerSSL();
            ServerConn.SSLHandshake();

            DBG(10, "SSL handshake completed.");

            ServerConn.resetRecvBuffer();
            ServerConn.ChunkEndReached = false;

        }

        //- on server idle timeout, initiate server reset (wait new connection)
        if (TimeoutObj.checkTimeoutReached() == true) {
            DBG(50, "Server idle timeout reached.");
            ServerConn.CloseConnection = true;
        }

        //- sleep to keep cpu time idle
        this_thread::sleep_for(chrono::milliseconds(1));

    }

    //- release ssl socket memory
    ServerConn.free();

    return 0;

}
