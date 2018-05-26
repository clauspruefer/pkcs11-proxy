#include "Client.hpp"


using namespace std;

void Client::processClient(string ClientUUID, shared_ptr<Connection<Filedescriptor>> ConHandler, SSLSocket *ServerConn)
{

    string ClientData;

    Timeout TimeoutObj;

    TimeoutObj.setTimeoutBlocking(false);
    TimeoutObj.setTimeout(SOCKET_IDLE_TIMEOUT_SEC, 0);

    DBG(10, "New client connection thread running UUID:" << ClientUUID << ". Loop on timeout.");

    //- try to get data until timeout reached
    while (TimeoutObj.checkTimeoutReached() == false) {

        DBG(200, "Enter client connection loop.");

        shared_ptr<Filedescriptor> FDObj(ConHandler->get(ClientUUID));

        if (FDObj == 0) {
            ERR("Client FD object non existent. Exiting thread.");
            break;
        }

        DBG(200, "Check recv data from local client connection.");

        //- get local connection data
        FDObj->recvDataRaw();

        //- if data was received
        if (FDObj->DataReceived) {

            //- reset timeout
            TimeoutObj.reset();

            stringstream SendData;

            ClientData = FDObj->Received;
            DBG(100, "Client data received:'"<< ClientData << "'.");

            SendData << ClientUUID << ClientData;

            DBG(100, "Send data with UUID:" << ClientUUID << " FDNr:" << FDObj->MainFD << " to server:'" << SendData.str() << "'.");

            bool SendError = false;

            ServerConn->lock();

            if (ServerConn->sendDataChunk(SendData.str()) == false) {
                DBG(40, "SSL Socket send failed.");
                SendError = true;
            }

            ServerConn->unlock();

            if (SendError == true) { break; }
        }

        if (FDObj->CloseConnection) {
            DBG(40, "Client Socket close received.");
            break;
        }

        //- sleep to keep cpu time idle
        this_thread::sleep_for(chrono::milliseconds(1));

    }

    DBG(40, "Close local connection.");

    shared_ptr<Filedescriptor> FDObj(ConHandler->get(ClientUUID));
    if (FDObj > 0) {
        FDObj->closeFD();
    }

    DBG(40, "Remove fd object from unordered_map.");

    //- remove uuid from connection map
    ConHandler->erase(ClientUUID);

    DBG(40, "Exit thread.");

}
