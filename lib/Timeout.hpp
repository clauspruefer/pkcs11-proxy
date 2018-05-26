#ifndef Timeout_hpp
#define Timeout_hpp

#include <sys/timerfd.h>

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>
#include <thread>

#include "Constant.hpp"


using namespace std;

class Timeout {

public:

    Timeout();
    ~Timeout();

    int getTimeoutFD();

    void setTimeout(unsigned int, long);
    void setTimeoutBlocking(const bool);
    void reset();

    bool checkTimeoutReached();

private:

    int TimerFD;
    bool Blocking;

    struct itimerspec TimeoutTimeNew;
};

#endif
