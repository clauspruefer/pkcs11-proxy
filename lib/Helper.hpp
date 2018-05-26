#ifndef Helper_hpp
#define Helper_hpp

#include "Constant.hpp"

#include "stdint.h"

#include <unistd.h>
#include <uuid/uuid.h>

#include <string>


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
        void* WriteAddress = &output;

        *(unsigned char*)WriteAddress = input.at(0);
        *(unsigned char*)(WriteAddress+1) = input.at(1);

        return (int)output;
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

#endif
