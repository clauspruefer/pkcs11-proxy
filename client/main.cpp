#include "Client.hpp"

#include "../lib/Config.cpp"
#include "../lib/Filedescriptor.cpp"
#include "../lib/Socket.cpp"
#include "../lib/SSLHandshake.cpp"
#include "../lib/SSLSocket.cpp"
#include "../lib/Connection.cpp"
#include "../lib/Timeout.cpp"


using namespace std;

int main()
{

    cout << "### >> ---------------------------------------------------------------" << endl;
    cout << "### >> TLS PKCS11 Proxy (Client component)" << endl;
    cout << "### >> ---------------------------------------------------------------" << endl;

    //- disable signals
    System::disableSignals();

    //- process xml configuration
    Config::setup();

    DBG(10, "Setup local listening socket.");

    //- setup local listening socket
    Socket LocalConn(Config::ListenHost, Config::ListenPort);
    LocalConn.setupSocket();
    LocalConn.setupServer();
    LocalConn.setSocketFDNonblocking();

    DBG(10, "Setup client ssl socket.");

    //- setup server ssl connection
    SSLSocket ServerConn(Config::RemoteHost, Config::RemotePort);
    ServerConn.setupSSL();
    ServerConn.setupSocket();
    ServerConn.setSocketFDNonblocking();

    DBG(10, "Wait for client connection.");

    //- initiate new client connection
    ServerConn.connectClient();

    //- do ssl handshake
    ServerConn.doClientHandshake();

    //- ---------------------------------------------------------------------------
    //- setup timeouts
    //- ---------------------------------------------------------------------------

    Timeout TimeoutRekeyObj;

    TimeoutRekeyObj.setTimeoutBlocking(false);
    TimeoutRekeyObj.setTimeout(CLIENT_REKEY_INTERVAL_SEC, 0);

    Timeout TimeoutKeepaliveObj;

    TimeoutKeepaliveObj.setTimeoutBlocking(false);
    TimeoutKeepaliveObj.setTimeout(SSLSOCKET_KEEPALIVE_PACKAGE_INTERVAL_SEC, 0);


    //- ---------------------------------------------------------------------------
    //- setup connection handler
    //- ---------------------------------------------------------------------------

    shared_ptr<Connection<Filedescriptor>> ConnHandler(new Connection<Filedescriptor>);


    //- ---------------------------------------------------------------------------
    //- loop until sigint received (main loop)
    //- ---------------------------------------------------------------------------

    bool RunClient = true;
    while (RunClient == true) {

        DBG(200, "Enter client loop. Waiting for new connection (accept).");

        //- setup new connection on accept
        int ClientFD = LocalConn.setupClientConnection();
        if (ClientFD > 0) {

            DBG(40, "Accepted client connection FDNr:" << ClientFD << ".");

            //- setup new fd object
            Filedescriptor *FDObj = new Filedescriptor();

            //- set fd in fd object
            FDObj->setFD(ClientFD);

            //- setup unique
            unique_ptr<Filedescriptor> ClientFDObj(FDObj);

            //- generate client uuid
            string UUID = UUID::generateUUID();

            //- send proxy handshake
            ServerConn.ProxyHandshake(UUID);

            //- add fd obj to map
            ConnHandler->add(UUID, move(ClientFDObj));

            //- fire new thread
            thread *t = new thread(&Client::processClient, UUID, ConnHandler, &ServerConn);

            //- detach thread
            t->detach();

        }

        //- process server data
        ServerConn.recvDataChunk();

        if (ServerConn.ChunkEndReached) {

            string ServerData = ServerConn.Received;

            ServerConn.ChunkEndReached = false;
            ServerConn.Received.clear();

            //- get client filedescriptor integer
            string ClientUUID = ServerData.substr(0, UUID_SIZE);

            DBG(200, "Received data for UUID:" << ClientUUID << " from Server:'" << ServerData << "'");

            //- remove uuid from send data
            ServerData.erase(0, UUID_SIZE);

            //- get fd object from connection map (locked)
            shared_ptr<Filedescriptor> FDObj(ConnHandler->get(ClientUUID));

            if (FDObj > 0) {

                //- close connection on server cmd, else send data to local client socket
                if (ServerData.compare(PROXY_CMD_CLOSE_CONNECTION) == 0) {

                    //- close filedescriptor
                    FDObj->closeFD();

                    //- remove fdobject from map
                    ConnHandler->erase(ClientUUID);
                }

                //- data
                else {
                    DBG(200, "Send data for UUID:" << ClientUUID << " FDNr:" << FDObj->MainFD << " to local socket:'" << ServerData << "'");
                    FDObj->sendData(ServerData);
                }
            }

            //- check rekey interval timeout reached
            if (TimeoutRekeyObj.checkTimeoutReached() == true) {

                DBG(50, "Rekey timeout reached.");

                //- reset timeout
                TimeoutRekeyObj.reset();

                //- renegotiate connection
                ServerConn.renegotiate();

            }

        }

        //- check keepalive package interval timeout reached
        if (TimeoutKeepaliveObj.checkTimeoutReached() == true) {

            DBG(50, "Keepalive Package timeout reached. Send Keepalive Package.");

            //- reset timeout
            TimeoutKeepaliveObj.reset();

            //- send keepalive packet
            stringstream SendData;
            SendData << UUID::generateUUID() << string(PROXY_CMD_KEEPALIVE);

            ServerConn.lock();

            if (ServerConn.sendDataChunk(SendData.str()) == false) {
                DBG(40, "Send keepalive packet failed, terminate client.");
                RunClient = false;
            }

            ServerConn.unlock();

        }

        //- close connection
        if (ServerConn.CloseConnection) {
            DBG(40, "Closing local ssl tunnel connection, terminate.");
            RunClient = false;
        }

        //- sleep to keep cpu time idle
        this_thread::sleep_for(chrono::milliseconds(1));

    }

    //- release ssl socket memory
    ServerConn.free();

    DBG(40, "Terminating client.");
    return 0;
}
