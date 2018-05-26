#ifndef Client_hpp
#define Client_hpp

#include <string>
#include <iostream>
#include <mutex>
#include <thread>

#include "../lib/Debug.cpp"

#include "../lib/Helper.hpp"
#include "../lib/Timeout.hpp"
#include "../lib/Socket.hpp"
#include "../lib/SSLSocket.hpp"
#include "../lib/Connection.hpp"

using namespace std;


class Client {

public:

    static void processClient(string, shared_ptr<Connection<Filedescriptor>>, SSLSocket*);

private:

};

#endif
