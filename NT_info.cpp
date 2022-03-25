#include "NT_info.h"

PIMAGE_NT_HEADERS NT_INFO::ntHeaders = nullptr;

bool NT_INFO::isPEHeader(PIMAGE_DOS_HEADER ImageBase) {
    /************************* e_lfanew stores the offset from ImageBase to IMAGE_NT_HEADER *************************/
    ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>((DWORD*)ImageBase + ImageBase->e_lfanew / 4);
    return ntHeaders->Signature == IMAGE_NT_SIGNATURE;
}
