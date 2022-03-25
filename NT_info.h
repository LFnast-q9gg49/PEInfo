#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#include <winnt.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#ifndef PEINFO_NT_INFO_H
#define PEINFO_NT_INFO_H

class NT_INFO{
public:
    static PIMAGE_NT_HEADERS ntHeaders;
    static bool isPEHeader(PIMAGE_DOS_HEADER);

};


#endif //PEINFO_NT_INFO_H