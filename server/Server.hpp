#ifndef Server_hpp
#define Server_hpp

#include <string>
#include <iostream>
#include <mutex>
#include <memory>
#include <thread>

#include "../lib/Debug.cpp"

#include "../lib/Helper.hpp"
#include "../lib/Constant.hpp"
#include "../lib/Timeout.hpp"
#include "../lib/Socket.hpp"
#include "../lib/SSLSocket.hpp"
#include "../lib/Connection.hpp"


using namespace std;

class Server {

public:

    static void processClient(const string, shared_ptr<Connection<Socket>>, SSLSocket*);

private:

};

#endif
