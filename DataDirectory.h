#include "Section_info.h"
#include "OptionalHeader.h"
#include "FileLoader.h"
#include <utility>

#ifndef PEINFO_DATADIRECTORY_H
#define PEINFO_DATADIRECTORY_H

static IMAGE_DATA_DIRECTORY DATA_DIRECTORY[16];
static vector<vector<DWORD>> SAE;
static IMAGE_IMPORT_DESCRIPTOR IM_DESC;


/**************************************************************/

// Because there are two condition of PE file (run in memory and stay in disk)
// Thus we need two different offset.
// R_ means RVA running in memory offset
// L_ means file offset in disk

/**************************************************************/

/**************************************************************/

/* struct of Import table*/
// OriginalFirstThunk ——————————————>  ThunkData ————————————————> Import By Name <———————————————— FirstThunk

/**************************************************************/

struct ExInfo{
    int id; // in which section can I find this data_directory
    DWORD R_AddressOfIED;
    DWORD F_AddressOfIED;
};

struct ImInfo{
    int id; // in which section can I find this data_directory
    DWORD R_AddressOfIID;
    DWORD F_AddressOfIID;
};

struct ReInfo{
    int id;
    DWORD R_AddressOfIID;
    DWORD F_AddressOfIID;
};

struct RsInfo{
    int id;
    DWORD R_AddressOfIID;
    DWORD F_AddressOfIID;
};

struct IED_info{
    DWORD F_NameAddr;
    DWORD R_NameAddr;
    DWORD Base;
    DWORD NumberOfFunctions;
    DWORD NumberOfNames;

    DWORD R_FuncName;
    DWORD F_FuncName;

    DWORD F_AddressOfFunctions;
    DWORD R_AddressOfFunctions;
    DWORD F_AddressOfNames;
    DWORD R_AddressOfNames;
    DWORD F_AddressOfNameOrdinals;
    DWORD R_AddressOfNameOrdinals;
};

struct IID_info{
    DWORD R_OriginalFirstThunk;
    DWORD F_OriginalFirstThunk;
    DWORD R_FirstThunk;
    DWORD F_FirstThunk;
    DWORD AddressOfImportByName;
    IMAGE_IMPORT_BY_NAME IBN;
};

struct ITD32_info{
    DWORD R_THUNK;
    DWORD F_THUNK;
};

struct ITD64_info{
    ULONGLONG R_THUNK;
    ULONGLONG F_THUNK;
};

struct BR_info{
    DWORD VirtualAddress;
    DWORD SizeOfBlock;
    DWORD numberOfRelocation;
};

class DD_INFO{
public:
    static bool InitDD(vector<vector<DWORD>> sae);
    static bool AnalyseImportTable();
    static bool AnalyseExportTable();
    static bool AnalyseResourceTable();
    static bool AnalyseBaseRelocationTable();
};

#endif //PEINFO_DATADIRECTORY_H