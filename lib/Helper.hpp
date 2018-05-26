#ifndef Helper_hpp
#define Helper_hpp

#include "Constant.hpp"

#include "stdint.h"

#include <unistd.h>
#include <uuid/uuid.h>

#include <string>
#include <csignal>


using namespace std;

class Conversion {

public:

    static string int2bytestring(int input)
    {
        uint16_t value = (uint16_t)input;
        string output;
        output.append((const char*)&value, sizeof(value));
        return output;
    }

    static int bytestring2int(string input)
    {

        if (input.length() < 2) {
            return -1;
        }

        uint16_t output;
        void* WriteAddressByte1 = &output;
        void* WriteAddressByte2 = &output+1;

        *static_cast<unsigned char*>(WriteAddressByte1) = input.at(0);
        *static_cast<unsigned char*>(WriteAddressByte2) = input.at(1);

        return static_cast<int>(output);
    }

};

class UUID {

    public:

    static string generateUUID()
    {
        char uuid_str[UUID_SIZE+1];
        uuid_t uuid;
        uuid_generate_time_safe(uuid);
        uuid_unparse_lower(uuid, uuid_str);
        string StringUUID(uuid_str, UUID_SIZE);

        DBG(200, "Generated UUID:" << StringUUID);

        return StringUUID;
    }
};

class System {

    public:

    static void disableSignals()
    {
        signal(SIGHUP,  SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        signal(SIGILL,  SIG_IGN);
        signal(SIGABRT, SIG_IGN);
    }

};

#endif
