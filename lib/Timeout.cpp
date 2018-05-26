#include "Timeout.hpp"

using namespace std;

Timeout::Timeout():
    TimerFD(-1),
    Blocking(true)
{
    DBG(200, "Timeout Constructor.");
    TimeoutTimeNew.it_value.tv_sec = 0;
    TimeoutTimeNew.it_value.tv_nsec = 0;
    TimeoutTimeNew.it_interval.tv_sec = 0;
    TimeoutTimeNew.it_interval.tv_nsec = 0;
}

Timeout::~Timeout()
{
    DBG(200, "Timeout Destructor.");
    close(TimerFD);
}

void Timeout::setTimeout(const unsigned int sec, const long nanosec) {

    DBG(20, "Timeout params - Seconds:" << sec << " Nanoseconds:" << nanosec);

    int TimeoutFlags = Blocking == false ? TFD_NONBLOCK : 0;

    TimerFD = timerfd_create(CLOCK_REALTIME, TimeoutFlags);

    TimeoutTimeNew.it_value.tv_sec = sec;
    TimeoutTimeNew.it_value.tv_nsec = nanosec;
    TimeoutTimeNew.it_interval.tv_sec = 0;
    TimeoutTimeNew.it_interval.tv_nsec = 0;

    int rc = timerfd_settime(TimerFD, 0, &TimeoutTimeNew, NULL);

    if (rc == -1) {
        ERR("Timeout settime failed: errno:" << errno << " error:" << strerror(errno));
    }

}

int Timeout::getTimeoutFD()
{
    return TimerFD;
}

bool Timeout::checkTimeoutReached()
{
    uint64_t TimeoutResult = 0;
    int rc = read(TimerFD, &TimeoutResult, sizeof(TimeoutResult));

    if (rc == -1) {
        DBG(200, "Read Error:" << errno << " ErrorString:" << strerror(errno));
    }

    return TimeoutResult > 0 ? true : false;
}

void Timeout::setTimeoutBlocking(const bool Flag)
{
    Blocking = Flag;
}

void Timeout::reset()
{
    int rc = timerfd_settime(TimerFD, 0, &TimeoutTimeNew, NULL);

    if (rc == -1) {
        ERR("Timeout reset settime failed: errno:" << errno << " error:" << strerror(errno));
    }
}
