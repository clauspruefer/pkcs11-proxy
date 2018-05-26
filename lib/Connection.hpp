#ifndef Connection_hpp
#define Connection_hpp

#include <string>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <memory>


using namespace std;

template <class T>
class Connection
{

    typedef unordered_map<string, shared_ptr<T>> Connections_t;
    typedef typename Connections_t::const_iterator ConnectionsIter_t;
    typedef pair<string, shared_ptr<T>> UUIDObj_t;

private:

    mutex ConnectionLock;
    Connections_t Connections;

public:

    Connection()
    {
        DBG(200, "Connection constructor called.");
    };

    ~Connection()
    {
        DBG(200, "Connection destructor called.");
    };

    shared_ptr<T> get(const string UUID)
    {

        ConnectionLock.lock();

        shared_ptr<T> RetVal;

        ConnectionsIter_t it = Connections.find(UUID);
        if (it != Connections.end()) {
            RetVal = it->second;
        }

        ConnectionLock.unlock();

        return RetVal;
    }

    void add(const string UUID, shared_ptr<T> Object)
    {
        ConnectionLock.lock();
        Connections.insert(UUIDObj_t(UUID, Object));
        ConnectionLock.unlock();
    }

    void erase(const string UUID)
    {

        ConnectionLock.lock();

        ConnectionsIter_t it = Connections.find(UUID);
        if (it != Connections.end()) {
            Connections.erase(it);
        }

        ConnectionLock.unlock();

    }

    void clear()
    {
        Connections.clear();
    }

};

#endif
