#include "DataDirectory.h"

/************************* Init some info, like acquiring the pointer to DATA_DIRECTORY *************************/
bool DD_INFO::InitDD(vector<vector<DWORD>> sae){
    OPTIONAL_HEADER::GetDataDirectoryTable(DATA_DIRECTORY);
    SAE = std::move(sae);
    return true;
}

/************************* range section table to find the position of the DATA_DIRECTORY *************************/
int GetPosition(DWORD address){
    int pos;
    int num = int(FILE_HEADER::GetSectionNumber());
    for (pos = 0; pos < num ; pos++){
        if (address >= SAE[pos][2] && address < SAE[pos][3]){
            return pos;
        }
    }
    return num+1;
}

/************************* I love this function so much hhh hhh *************************/
bool DD_INFO::AnalyseExportTable() {
    auto* ex_info = new ExInfo;
    auto* IED = new IED_info;
    ex_info->R_AddressOfIED = DATA_DIRECTORY[0].VirtualAddress;
    if (ex_info->R_AddressOfIED == 0 || DATA_DIRECTORY[0].Size == 0){
        cout << "this file doesn't have export table" << '\n';
        return false;
    }
    ex_info->id = GetPosition(DATA_DIRECTORY[0].VirtualAddress);

    ex_info->F_AddressOfIED = SAE[ex_info->id][0] + ex_info->R_AddressOfIED - SAE[ex_info->id][2];


    auto* ied = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>((DWORD *) FileLoader::ImageBase + ex_info->F_AddressOfIED / 4);
    IED->Base = ied->Base;
    IED->R_NameAddr = ied->Name;
    IED->R_AddressOfNames = ied->AddressOfNames;
    IED->R_AddressOfNameOrdinals = ied->AddressOfNameOrdinals;
    IED->R_AddressOfFunctions = ied->AddressOfFunctions;
    IED->NumberOfNames = ied->NumberOfNames;
    IED->NumberOfFunctions = ied->NumberOfFunctions;

    // start with the address of names
    IED->F_AddressOfNames = SAE[ex_info->id][0] + IED->R_AddressOfNames - SAE[ex_info->id][2];
    auto* names = (DWORD *) FileLoader::ImageBase + IED->F_AddressOfNames / 4;

    IED->F_AddressOfNameOrdinals = SAE[ex_info->id][0] + IED->R_AddressOfNameOrdinals - SAE[ex_info->id][2];
    auto* nameOrd = ((WORD *) FileLoader::ImageBase + IED->F_AddressOfNameOrdinals / 2);

    IED->F_AddressOfFunctions = SAE[ex_info->id][0] + IED->R_AddressOfFunctions - SAE[ex_info->id][2];
    auto* func = (DWORD *) FileLoader::ImageBase + IED->F_AddressOfFunctions / 4;
    DWORD EntryPointOfFunction[IED->NumberOfNames];
    for (auto j = 0; j < IED->NumberOfFunctions; j++){
        EntryPointOfFunction[j] = *func;
        func++;
    }

    for (auto i = 0; i < IED->NumberOfNames; i++){
        /************************* calculate the true offset of func_name *************************/
        IED->R_FuncName = DWORD(*names);
        IED->F_FuncName = SAE[ex_info->id][0] + IED->R_FuncName - SAE[ex_info->id][2];

        /************************* bind with ImageBase *************************/
        auto trueName = (BYTE*) FileLoader::ImageBase + IED->F_FuncName;

        while(true){
            if (*trueName == '\0'){
                break;
            }
            cout << *trueName;
            trueName++;
        }
        cout << '\t' << "No. of this function is : " << *nameOrd << '\t' << "EntryPointOfFunction is : " << EntryPointOfFunction[*nameOrd];
        cout << '\n';
        nameOrd++;
        names++;
    }

    return true;
}

bool DD_INFO::AnalyseImportTable() {
    auto* im_info = new ImInfo;
    im_info->R_AddressOfIID = DATA_DIRECTORY[1].VirtualAddress;
    if (im_info->R_AddressOfIID == 0 || DATA_DIRECTORY[1].Size == 0){
        cout << "this file doesn't have import table";
        return false;
    }
    im_info->id = GetPosition(im_info->R_AddressOfIID);

    /************************* where can i find Import descriptor *************************/
    im_info->F_AddressOfIID = SAE[im_info->id][0] + im_info->R_AddressOfIID - SAE[im_info->id][2];
    auto* IID = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR *>((DWORD *) FileLoader::ImageBase + im_info->F_AddressOfIID / 4);

    /************************* range Import Descriptor *************************/
    while (IID){
        /************************* five zero in a row means the end *************************/
        if (!(IID->OriginalFirstThunk | IID->TimeDateStamp | IID->ForwarderChain | IID->Name | IID->FirstThunk)){
            break;
        }
        auto *info = new IID_info;

        /************************* F_OriginalFirstThunk is the offset of ThunkData *************************/
        info->R_OriginalFirstThunk = IID->OriginalFirstThunk;
        info->R_FirstThunk = IID->FirstThunk;
        info->F_OriginalFirstThunk = SAE[im_info->id][0] + info->R_OriginalFirstThunk - SAE[im_info->id][2];

        /************************* According to your machine to get different ThunkData *************************/
        if (FILE_HEADER::JudgeMachine()->bit_info){
            auto *ITD32 = new ITD32_info;
            auto* itd = reinterpret_cast<IMAGE_THUNK_DATA32 *>((DWORD *) FileLoader::ImageBase +  info->F_OriginalFirstThunk/ 4);

            ITD32->R_THUNK = itd->u1.AddressOfData;
            ITD32->F_THUNK = SAE[im_info->id][0] + ITD32->R_THUNK - SAE[im_info->id][2];

            info->AddressOfImportByName = ITD32->F_THUNK;
        }else {
            auto *ITD64 = new ITD64_info;
            auto* itd = reinterpret_cast<IMAGE_THUNK_DATA64 *>((DWORD *) FileLoader::ImageBase +  info->F_OriginalFirstThunk/ 4);

            ITD64->R_THUNK = itd->u1.AddressOfData;
            ITD64->F_THUNK = SAE[im_info->id][0] + ITD64->R_THUNK - SAE[im_info->id][2];

            info->AddressOfImportByName = ITD64->F_THUNK;
        }

        auto *ibn = reinterpret_cast<IMAGE_IMPORT_BY_NAME *>((BYTE *) FileLoader::ImageBase + info->AddressOfImportByName);
        info->IBN = *ibn;
        cout << ibn->Hint << '\t' << ibn->Name << '\n';
        IID++;
    }
    return true;
}

bool DD_INFO::AnalyseResourceTable(){
    auto* rs_info = new RsInfo;
    rs_info->R_AddressOfIID = DATA_DIRECTORY[2].VirtualAddress;
    if (rs_info->R_AddressOfIID == 0 || DATA_DIRECTORY[2].Size == 0){
        cout << "this file doesn't have base relocation table";
        return false;
    }
    rs_info->id = GetPosition(DATA_DIRECTORY[2].VirtualAddress);
    rs_info->F_AddressOfIID = SAE[rs_info->id][0] + rs_info->R_AddressOfIID - SAE[rs_info->id][2];
}

bool DD_INFO::AnalyseBaseRelocationTable() {
    auto* re_info = new ReInfo;
    auto* BR_INFO = new BR_info;
    re_info->R_AddressOfIID = DATA_DIRECTORY[5].VirtualAddress;
    if (re_info->R_AddressOfIID == 0 || DATA_DIRECTORY[5].Size == 0){
        cout << "this file doesn't have base relocation table";
        return false;
    }
    re_info->id = GetPosition(DATA_DIRECTORY[5].VirtualAddress);
    re_info->F_AddressOfIID = SAE[re_info->id][0] + re_info->R_AddressOfIID - SAE[re_info->id][2];
    auto* br = reinterpret_cast<IMAGE_BASE_RELOCATION *>((DWORD *) FileLoader::ImageBase + re_info->F_AddressOfIID / 4);
    BR_INFO->VirtualAddress = br->VirtualAddress;
    BR_INFO->SizeOfBlock = br->SizeOfBlock;

    /************************* Relocation list end with a WORD 0x00 *************************/
    BR_INFO->numberOfRelocation = (BR_INFO->SizeOfBlock - 8) / 2;

}