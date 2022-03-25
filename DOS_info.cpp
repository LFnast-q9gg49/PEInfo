#include "DOS_info.h"

PIMAGE_DOS_HEADER DOS_INFO::dosHeader = nullptr;

bool DOS_INFO::isMZHeader(void* ImageBase) {
    dosHeader = (PIMAGE_DOS_HEADER) ImageBase;

    /************************* if e_magic is 0x5a4d (ZM), that means this is a dos file *************************/
    return dosHeader->e_magic == IMAGE_DOS_SIGNATURE;
}