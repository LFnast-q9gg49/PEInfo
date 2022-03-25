#include "File_header.h"

// Only common processors are listed here. You can query and add them if necessary
MachineInfo* FILE_HEADER::JudgeMachine(){
    auto* m_info = new(MachineInfo);
    switch (NT_INFO::ntHeaders->FileHeader.Machine) {
        case IMAGE_FILE_MACHINE_I386:
            m_info->Name = "x86-I386(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_AMD64:
            m_info->Name = "AMD64(64bit)";
            m_info->bit_info = BIT64;
            break;
        case IMAGE_FILE_MACHINE_R3000:
            m_info->Name = "MIPS_R3000(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_R4000:
            m_info->Name = "MIPS_R4000(64bit)";
            m_info->bit_info = BIT64;
            break;
        case IMAGE_FILE_MACHINE_R10000:
            m_info->Name = "MIPS_R10000(64bit)";
            m_info->bit_info = BIT64;
            break;
        case IMAGE_FILE_MACHINE_SH3:
            m_info->Name = "SuperH3(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_SH4:
            m_info->Name = "SuperH4(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_SH5:
            m_info->Name = "SuperH5(64bit)";
            m_info->bit_info = BIT64;
            break;
        case IMAGE_FILE_MACHINE_ARM:
            m_info->Name = "ARM(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_ARMV7:
            m_info->Name = "ARMV7(32bit)";
            m_info->bit_info = BIT32;
            break;
        case IMAGE_FILE_MACHINE_ARM64:
            m_info->Name = "ARM64(64bit)";
            m_info->bit_info = BIT64;
            break;
        case IMAGE_FILE_MACHINE_IA64:
            m_info->Name = "IA64(64bit)";
            m_info->bit_info = BIT64;
            break;
        default:
            m_info->Name = "can't judge the machine";
            m_info->bit_info = BIT64;
    }
    return m_info;
}

DWORD FILE_HEADER::GetSectionNumber() {
    return NT_INFO::ntHeaders->FileHeader.NumberOfSections;
}

/************************* a timestamp parser *************************/
// base is 1900/1/1/ 00:00:00
string FILE_HEADER::TimeParser() {
    time_t time_stamp = NT_INFO::ntHeaders->FileHeader.TimeDateStamp;
    tm t_result = *localtime(&time_stamp);

    string result = to_string(1900 + t_result.tm_year ) + "/" + to_string(1 + t_result.tm_mon ) + "/" + to_string(t_result.tm_mday) + "\t" +
                    to_string(t_result.tm_hour) + ":" + to_string(t_result.tm_min) + ":" + to_string(t_result.tm_sec);

    return result;
}

/************************* get the size of OptionalHeader *************************/
DWORD FILE_HEADER::GetOpMem() {
    return NT_INFO::ntHeaders->FileHeader.SizeOfOptionalHeader;
}

string FILE_HEADER::JudgeFileChara() {
    string CHARACTERISTICS[15] = {
            "IMAGE_FILE_RELOCS_STRIPPED",
            "IMAGE_FILE_EXECUTABLE_IMAGE",
            "IMAGE_FILE_LINE_NUMS_STRIPPED",
            "IMAGE_FILE_LOCAL_SYMS_STRIPPED",
            "IMAGE_FILE_AGGRESIVE_WS_TRIM",
            "IMAGE_FILE_LARGE_ADDRESS_AWARE",
            "IMAGE_FILE_BYTES_REVERSED_LO",
            "IMAGE_FILE_32BIT_MACHINE",
            "IMAGE_FILE_DEBUG_STRIPPED",
            "IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP",
            "IMAGE_FILE_NET_RUN_FROM_SWAP",
            "IMAGE_FILE_SYSTEM",
            "IMAGE_FILE_DLL",
            "IMAGE_FILE_UP_SYSTEM_ONLY",
            "IMAGE_FILE_BYTES_REVERSED_HI"};

    string result;
    DWORD Characteristic = NT_INFO::ntHeaders->FileHeader.Characteristics;
    for (int i = 0; Characteristic > ( 1 << i ) ; ++i){
        if (Characteristic & (1 << i)){
            result += (CHARACTERISTICS[i] + '\t');
        }
    }
    return result;
}