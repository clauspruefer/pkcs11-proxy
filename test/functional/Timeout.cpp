#include <arpa/inet.h>
#include <stdio.h>

#include <string>
#include <iostream>

#include "../../lib/Debug.cpp"
#include "../../lib/Timeout.cpp"

using namespace std;


//- main
int main()
{

    //- setup stack timeout obj
    Timeout TimeoutObj;

    //- set first timeout
    TimeoutObj.setTimeout(10, 0);

    while(true) {
        if (TimeoutObj.checkTimeoutReached() == true) {
            break;
        }
    }

    cout << "Timeout1 (10sec) reached." << endl;

    //- set second timeout
    TimeoutObj.setTimeout(20, 0);

    while(true) {
        if (TimeoutObj.checkTimeoutReached() == true) {
            break;
        }
    }

    cout << "Timeout2 (20sec) reached." << endl;

    return 0;

}
