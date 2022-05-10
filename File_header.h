#include "NT_info.h"

#ifndef PEINFO_FILE_HEADER_H
#define PEINFO_FILE_HEADER_H

const int BIT32 = true;
const int BIT64 = false;

class MachineInfo{
public:
    const CHAR *Name;
    bool bit_info;
};

class FILE_HEADER{

public:
    static MachineInfo* JudgeMachine();
    static DWORD GetSectionNumber();
    static string TimeParser();
    static DWORD GetOpMem();
    static string JudgeFileChara();
};

#endif //PEINFO_FILE_HEADER_H
