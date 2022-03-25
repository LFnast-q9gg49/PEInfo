#include "OptionalHeader.h"

// 32bits
bool OPTIONAL_HEADER::GetOP32InfoWithOutDataDirectoryTable(OP32_INFO* op32Info) {
    auto optionalHeader = reinterpret_cast<PIMAGE_NT_HEADERS32>((DWORD*)DOS_INFO::dosHeader + DOS_INFO::dosHeader->e_lfanew / 4)->OptionalHeader;
    if (optionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC){
        return false;
    }

    op32Info->AddressOfEntryPoint = optionalHeader.AddressOfEntryPoint;
    op32Info->FileAlignment = optionalHeader.FileAlignment;
    op32Info->SectionAlignment = optionalHeader.SectionAlignment;
    op32Info->ImageBase = optionalHeader.ImageBase;

    return true;
}

// 64bits
bool OPTIONAL_HEADER::GetOP64InfoWithOutDataDirectoryTable(OP64_INFO* op64Info) {
    auto optionalHeader = reinterpret_cast<PIMAGE_NT_HEADERS64>((DWORD*)DOS_INFO::dosHeader + DOS_INFO::dosHeader->e_lfanew / 4)->OptionalHeader;
    if (optionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC){
        return false;
    }

    op64Info->AddressOfEntryPoint = optionalHeader.AddressOfEntryPoint;
    op64Info->FileAlignment = optionalHeader.FileAlignment;
    op64Info->SectionAlignment = optionalHeader.SectionAlignment;
    op64Info->ImageBase = optionalHeader.ImageBase;

    return true;
}

/************************* just a copy function *************************/
bool OPTIONAL_HEADER::GetDataDirectoryTable(PIMAGE_DATA_DIRECTORY dataDirectory){
    if (FILE_HEADER::JudgeMachine()->bit_info){
        auto optionalHeader = reinterpret_cast<PIMAGE_NT_HEADERS32>((DWORD*)DOS_INFO::dosHeader + DOS_INFO::dosHeader->e_lfanew / 4)->OptionalHeader;
        for (int i = 0; i < 16; i++){
            dataDirectory[i].VirtualAddress = optionalHeader.DataDirectory[i].VirtualAddress;
            dataDirectory[i].Size = optionalHeader.DataDirectory[i].Size;
        }
    } else{
        auto optionalHeader = reinterpret_cast<PIMAGE_NT_HEADERS64>((DWORD*)DOS_INFO::dosHeader + DOS_INFO::dosHeader->e_lfanew / 4)->OptionalHeader;
        for (int i = 0; i < 16; i++){
            dataDirectory[i].VirtualAddress = optionalHeader.DataDirectory[i].VirtualAddress;
            dataDirectory[i].Size = optionalHeader.DataDirectory[i].Size;
        }
    }
    return true;
}