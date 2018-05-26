#include <arpa/inet.h>
#include <stdio.h>

#include <string>
#include <iostream>

using namespace std;

//- main
int main()
{

    int fd = 745;
    uint16_t fd2 = (uint16_t)fd;

    void* FDAddress = &fd2;

    unsigned char b1 = *(unsigned char*)FDAddress;
    unsigned char b2 = *(unsigned char*)(FDAddress+1);

    printf("%x:%x\n", b1, b2);

    string test;
    test.append((const char*)&fd2, sizeof(fd2));

    cout << test << endl;
    cout << test.length() << endl;

    printf("%x:%x\n", test.at(0), test.at(1));

    uint16_t fd3;
    void* FDWriteAddress = &fd3;

    *(unsigned char*)FDWriteAddress = test.at(0);
    *(unsigned char*)(FDWriteAddress+1) = test.at(1);

    cout << "Written uint16_t:" << fd3 << endl;
    return 0;

}
