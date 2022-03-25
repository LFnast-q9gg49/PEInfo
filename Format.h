#include "NT_info.h"

#ifndef PEINFO_FORMAT_H
#define PEINFO_FORMAT_H

class FORMAT{
public:
    static string WordToHex(WORD);
    static string DwordToHex(DWORD);
};

#endif //PEINFO_FORMAT_H
