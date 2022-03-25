#include "File_header.h"
#include "DOS_info.h"

#ifndef PEINFO_OPTIONALHEADER_H
#define PEINFO_OPTIONALHEADER_H

/************************* As you can see, 32bit and 64bit has different length of ImageBase *************************/
/************************* what I can do is to build two struct *************************/
struct OP32_INFO{
    DWORD                AddressOfEntryPoint; // VA
    DWORD                ImageBase; // 基址
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
};

struct OP64_INFO{
    DWORD                AddressOfEntryPoint; // VA
    ULONGLONG            ImageBase; // 基址
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
};

class OPTIONAL_HEADER{
public:
    static bool GetOP32InfoWithOutDataDirectoryTable(OP32_INFO* op32Info);
    static bool GetOP64InfoWithOutDataDirectoryTable(OP64_INFO* op64Info);
    static bool GetDataDirectoryTable(PIMAGE_DATA_DIRECTORY dataDirectory);

};

#endif //PEINFO_OPTIONALHEADER_H