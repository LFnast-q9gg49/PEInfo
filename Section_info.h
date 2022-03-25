#include "NT_info.h"
#include "File_header.h"

#ifndef PEINFO_SECTION_INFO_H
#define PEINFO_SECTION_INFO_H

struct SECTION{
    BYTE Name[8];
    DWORD PhysicalAddress; // not spec
    DWORD VirtualAddress; // virtual start
    DWORD SizeOfRawData; // spec
    DWORD PointerToRawData; // file position
    vector<string> Characteristics;
};

class SECTION_INFO{
public:
    static PIMAGE_SECTION_HEADER GetFirstSection();
    static bool GetSectionInfo(vector<SECTION> &sec);
    static bool JudgeCharacter(DWORD, vector<string> &ret);

    /************************* we do need this function *************************/
    static bool CalculateStartAndEnd(DWORD sectionAlignment, vector<vector<DWORD>> &sae);
};

#endif //PEINFO_SECTION_INFO_H
