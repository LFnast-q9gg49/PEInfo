#include "Section_info.h"

PIMAGE_SECTION_HEADER SECTION_INFO::GetFirstSection() {
    return IMAGE_FIRST_SECTION(NT_INFO::ntHeaders);
}

/************************* a copy function again *************************/
bool SECTION_INFO::GetSectionInfo(vector<SECTION> &sec) {
    int num = int(FILE_HEADER::GetSectionNumber());
    PIMAGE_SECTION_HEADER ptr = GetFirstSection();
    auto* newSec = new(SECTION);
    for (int i = 0; i < num; ++i){
        vector<string> ret;
        copy(begin(ptr->Name), end(ptr->Name), begin(newSec->Name));
        newSec->PhysicalAddress = ptr->Misc.PhysicalAddress;
        newSec->VirtualAddress = ptr->VirtualAddress;
        newSec->SizeOfRawData = ptr->SizeOfRawData;
        newSec->PointerToRawData = ptr->PointerToRawData;

        JudgeCharacter(ptr->Characteristics, ret);

        newSec->Characteristics = ret;
        sec.push_back(*newSec);
        ++ptr;
    }
    return true;
}

/************************* some simple bit operation *************************/
bool SECTION_INFO::JudgeCharacter(DWORD chara, vector<string> &retInner) {

    // Chara part.1
    string Chara1[15] = {
            " IMAGE_SCN_CNT_CODE",
            " IMAGE_SCN_CNT_INITIALIZED_DATA",
            " IMAGE_SCN_CNT_UNINITIALIZED_DATA",
            " IMAGE_SCN_LNK_OTHER",
            " IMAGE_SCN_LNK_INFO",
            " There should be no value here, how can you get this",
            " IMAGE_SCN_LNK_REMOVE",
            " IMAGE_SCN_LNK_COMDAT",
            " There should be no value here, how can you get this",
            " IMAGE_SCN_NO_DEFER_SPEC_EXC",
            " IMAGE_SCN_GPREL   IMAGE_SCN_MEM_FARDATA",
            " There should be no value here, how can you get this",
            " IMAGE_SCN_MEM_PURGEABLE   IMAGE_SCN_MEM_16BIT",
            " IMAGE_SCN_MEM_LOCKED",
            " IMAGE_SCN_MEM_PRELOAD"
    };
    DWORD chara1 = chara & 0x000FFFFF;
    for (int i = 0; chara1 >= (1 << (i+5)); i++){
        if ( chara1 & (1 << (i+5))){
            retInner.push_back(Chara1[i]);
        }
    }


    // Chara part.2 : Align Characteristic
    DWORD chara2 = chara & 0x00F00000;

    if (chara2 == 0x00F00000){
        retInner.emplace_back(" ALIGN_BYTE_MASK");
    }else if (chara2 != 0){
        retInner.push_back(to_string(1 << ((chara2 >> 20) - 1)) + "BYTE");
    }


    // Chara part.3
    string Chara3[8] = {
            " IMAGE_SCN_LNK_NRELOC_OVFL",
            " IMAGE_SCN_MEM_DISCARDABLE",
            " IMAGE_SCN_MEM_NOT_CACHED",
            " IMAGE_SCN_MEM_NOT_PAGED",
            " IMAGE_SCN_MEM_SHARED",
            " IMAGE_SCN_MEM_EXECUTE",
            " IMAGE_SCN_MEM_READ",
            " IMAGE_SCN_MEM_WRITE",
    };
    string res3;
    DWORD chara3 = chara & 0xFF000000;
    for (int i = 0; (chara >= (1 << (i+24))) && (0x80000000 > 1 << ( i + 24)) ; i++){
        if (chara3 & (1 << (i+24))){
            retInner.push_back(Chara3[i]);
        }
    }

    return true;
}

bool SECTION_INFO::CalculateStartAndEnd(DWORD sectionAlignment, vector<vector<DWORD>> &sae) {
    int num = int(FILE_HEADER::GetSectionNumber());
    DWORD temp = 0;
    PIMAGE_SECTION_HEADER ptr = GetFirstSection();
    vector<DWORD> ret;
    for (int i = 0; i < num; ++i){
        ret.push_back(ptr->PointerToRawData);
        ret.push_back(ptr->PointerToRawData + ptr->SizeOfRawData);
        ret.push_back(ptr->VirtualAddress);
        // based on virtual alignment in optional header;
        temp = ptr->VirtualAddress + (ptr->SizeOfRawData / sectionAlignment) * sectionAlignment ;
        if(ptr->SizeOfRawData % sectionAlignment != 0){
            temp += sectionAlignment;
        }
        ret.push_back(temp);
        sae.push_back(ret);
        ret.clear();
        ++ptr;
    }
    return true;
}