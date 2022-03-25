#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#include <winnt.h>

#ifndef PEINFO_DOS_INFO_H
#define PEINFO_DOS_INFO_H

class DOS_INFO{

public:
    static bool isMZHeader(void*);
    static PIMAGE_DOS_HEADER dosHeader;

};

#endif //PEINFO_DOS_INFO_H
