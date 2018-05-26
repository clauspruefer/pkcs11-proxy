#include <arpa/inet.h>
#include <stdio.h>

#include <string>
#include <iostream>

#include "../../lib/Socket.cpp"
#include "../../lib/Connection.cpp"
#include "../../lib/Filedescriptor.cpp"

using namespace std;

//- main
int main()
{

    unique_ptr<Filedescriptor> p1(new Filedescriptor());

    unique_ptr<Connection<Filedescriptor>> ConnHandler(new Connection<Filedescriptor>);

    ConnHandler->add(string("123456789"), move(p1));

    cout << ConnHandler->get(string("123456789")) << endl;

    ConnHandler->erase(string("123456789"));
    ConnHandler->erase(string("123456789"));

    cout << ConnHandler->get(string("123456789")) << endl;

    shared_ptr<Filedescriptor> FDObj1(ConnHandler->get("123456789"));

    if (FDObj1 == 0) {
        cout << "yo" << endl;
    }

    shared_ptr<Filedescriptor> FDObj2(ConnHandler->get("123456789"));

    if (FDObj2 == 0) {
        cout << "yo" << endl;
    }
}
