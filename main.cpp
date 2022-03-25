#include <bits/stdc++.h>
#include "FileLoader.h"
#include "DOS_info.h"
#include "NT_info.h"
#include "File_header.h"
#include "OptionalHeader.h"
#include "Section_info.h"
#include "DataDirectory.h"
#include "Format.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    DWORD sectionAlignment;
    char dst[] = "D:\\wecapi.dll";

    if ((FileLoader::Get_hFile(dst) & ERRORCODE) == ERRORCODE){
        printf("Open file error");
        system("pause");
        return 0;
    }

    if ((FileLoader::Get_hMapping() & ERRORCODE) == ERRORCODE){
        printf("Failed to create file mapping kernel pair object");
        system("pause");
        return 0;
    }

    if ((FileLoader::Get_ImageBase() & ERRORCODE) == ERRORCODE){
        printf("Mapping to process address space failed");
        system("pause");
        return 0;
    }

    if (!DOS_INFO::isMZHeader(FileLoader::ImageBase)){
        printf("this file is not a PE file");
        system("pause");
        return 0;
    }

    if (!NT_INFO::isPEHeader(DOS_INFO::dosHeader)){
        printf("this file is not a PE file");
        system("pause");
        return 0;
    }

    // IMAGE_FILE_HEADER
    auto* machineInfo = new MachineInfo;
    machineInfo = FILE_HEADER::JudgeMachine();
    cout << machineInfo->Name << '\t' << machineInfo->bit_info << '\n';

    DWORD numberOfSection = FILE_HEADER::GetSectionNumber();
    cout << "this file has " << numberOfSection << " sections" << '\n';

    string res1 = FILE_HEADER::TimeParser();
    cout << "this file is created on " << res1 << '\n';

    DWORD sizeOfOptionalHeader = FILE_HEADER::GetOpMem();

    cout << "the size of OptionalHeader is " << FORMAT::WordToHex(sizeOfOptionalHeader) << endl;

    string res2 = FILE_HEADER::JudgeFileChara();
    cout << "there are characteristics of this file : " << res2 << endl;


    if (machineInfo->bit_info) {
        auto* op32Info = new OP32_INFO;
        OPTIONAL_HEADER::GetOP32InfoWithOutDataDirectoryTable(op32Info);
        sectionAlignment = op32Info->SectionAlignment;

        cout << "File Alignment : " << FORMAT::DwordToHex(op32Info->FileAlignment) << '\n';
        cout << "Section Alignment : " << FORMAT::DwordToHex(op32Info->SectionAlignment) << '\n';
        cout << "ImageBase : " << FORMAT::DwordToHex(op32Info->ImageBase) << '\n';
        cout << "Address of entrypoint : " << FORMAT::DwordToHex(op32Info->AddressOfEntryPoint) << '\n';
    } else{
        auto* op64Info = new OP64_INFO;
        OPTIONAL_HEADER::GetOP64InfoWithOutDataDirectoryTable(op64Info);
        sectionAlignment = op64Info->SectionAlignment;

        cout << "File Alignment : " << FORMAT::DwordToHex(op64Info->FileAlignment) << '\n';
        cout << "Section Alignment : " << FORMAT::DwordToHex(op64Info->SectionAlignment) << '\n';
        cout << "ImageBase : " << FORMAT::DwordToHex(op64Info->ImageBase) << '\n';
        cout << "Address of entrypoint : " << FORMAT::DwordToHex(op64Info->AddressOfEntryPoint) << '\n';
    }

    // SectionTable

    vector<SECTION> sec;
    vector<vector<DWORD>> sae;
    SECTION_INFO::GetSectionInfo(sec);
    SECTION_INFO::CalculateStartAndEnd(sectionAlignment, sae);

    for (auto i = 0; i < numberOfSection; i++){
        cout << sec[i].Name << '\t' << FORMAT::DwordToHex(sec[i].PhysicalAddress) << '\t' << FORMAT::DwordToHex(sec[i].VirtualAddress)
             << '\t' << FORMAT::DwordToHex(sec[i].SizeOfRawData) << '\t' << FORMAT::DwordToHex(sec[i].PointerToRawData) << '\t' << '\n';

        for (auto &m : sae[i]){
            cout << FORMAT::DwordToHex(m) << '\t';
        }
        cout << endl;

        for (auto &j : sec[i].Characteristics){
            cout << j << '\t';
        }
        cout << '\n';
    }

    // Data_Directory_Table
    DD_INFO::InitDD(sae);
    DD_INFO::AnalyseExportTable();
    DD_INFO::AnalyseImportTable();
    DD_INFO::AnalyseBaseRelocationTable();

    MessageBox(nullptr, TEXT("this is demo"), TEXT("this is demooooooo"), MB_OKCANCEL);

    return 0;
}