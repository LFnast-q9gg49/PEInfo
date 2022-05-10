#include "Analyze.h"

static vector<vector<string>> secInfo;
static int NOS;

bool Analyze::Ana(HWND hwnd, char* dst) {
    DWORD sectionAlignment;

    if ((FileLoader::Get_hFile(dst) & ERRORCODE) == ERRORCODE){
        printf("Open file error");
        system("pause");
        return false;
    }

    if ((FileLoader::Get_hMapping() & ERRORCODE) == ERRORCODE){
        printf("Failed to create file mapping kernel pair object");
        system("pause");
        return false;
    }

    if ((FileLoader::Get_ImageBase() & ERRORCODE) == ERRORCODE){
        printf("Mapping to process address space failed");
        system("pause");
        return false;
    }

    if (!DOS_INFO::isMZHeader(FileLoader::ImageBase)){
        printf("this file is not a PE file");
        system("pause");
        return false;
    }

    if (!NT_INFO::isPEHeader(DOS_INFO::dosHeader)){
        printf("this file is not a PE file");
        system("pause");
        return false;
    }

    // IMAGE_FILE_HEADER
    auto* machineInfo = new MachineInfo;
    machineInfo = FILE_HEADER::JudgeMachine();
    SetDlgItemText(hwnd, 1002, machineInfo->Name);

    DWORD numberOfSection = FILE_HEADER::GetSectionNumber();
    NOS = numberOfSection;
    char numOs[10];
    StringCchPrintfA(numOs,10,"%d", numberOfSection);
    SetDlgItemText(hwnd, 1003, numOs);

    string res1 = FILE_HEADER::TimeParser();
    SetDlgItemText(hwnd, 1004, res1.c_str());

    DWORD sizeOfOptionalHeader = FILE_HEADER::GetOpMem();
    SetDlgItemText(hwnd, 1005, FORMAT::WordToHex(sizeOfOptionalHeader).c_str());

    string res2 = FILE_HEADER::JudgeFileChara();
    SetDlgItemText(hwnd, 1006, res2.c_str());


    if (machineInfo->bit_info) {
        auto* op32Info = new OP32_INFO;
        OPTIONAL_HEADER::GetOP32InfoWithOutDataDirectoryTable(op32Info);
        sectionAlignment = op32Info->SectionAlignment;
        SetDlgItemText(hwnd, 1007, FORMAT::DwordToHex(op32Info->FileAlignment).c_str());
        SetDlgItemText(hwnd, 1008, FORMAT::DwordToHex(op32Info->SectionAlignment).c_str());
        SetDlgItemText(hwnd, 1009, FORMAT::DwordToHex(op32Info->ImageBase).c_str());
        SetDlgItemText(hwnd, 1010, FORMAT::DwordToHex(op32Info->AddressOfEntryPoint).c_str());
    } else {
        auto *op64Info = new OP64_INFO;
        OPTIONAL_HEADER::GetOP64InfoWithOutDataDirectoryTable(op64Info);
        sectionAlignment = op64Info->SectionAlignment;

        SetDlgItemText(hwnd, 1007, FORMAT::DwordToHex(op64Info->FileAlignment).c_str());
        SetDlgItemText(hwnd, 1008, FORMAT::DwordToHex(op64Info->SectionAlignment).c_str());
        SetDlgItemText(hwnd, 1009, FORMAT::DwordToHex(op64Info->ImageBase).c_str());
        SetDlgItemText(hwnd, 1010, FORMAT::DwordToHex(op64Info->AddressOfEntryPoint).c_str());
    }

    vector<SECTION> sec;
    vector<vector<DWORD>> sae;
    SECTION_INFO::GetSectionInfo(sec);
    SECTION_INFO::CalculateStartAndEnd(sectionAlignment, sae);

    for (auto i = 0; i < numberOfSection; i++){
        vector<string> secInfotemp;
        secInfotemp.emplace_back((char*)sec[i].Name);

        secInfotemp.emplace_back(FORMAT::DwordToHex(sec[i].PhysicalAddress));
        secInfotemp.emplace_back(FORMAT::DwordToHex(sec[i].VirtualAddress));
        secInfotemp.emplace_back(FORMAT::DwordToHex(sec[i].SizeOfRawData));
        secInfotemp.emplace_back(FORMAT::DwordToHex(sec[i].PointerToRawData));

        for (auto &m : sae[i]){
            secInfotemp.emplace_back(FORMAT::DwordToHex(m));
        }

        string tmp;
        for (auto &j : sec[i].Characteristics){
            tmp += j + "    ";
        }
        secInfotemp.emplace_back(tmp);
        secInfo.emplace_back(secInfotemp);
        secInfotemp.clear();
    }
    /*
    // SectionTable

    // Data_Directory_Table
    DD_INFO::InitDD(sae);
    DD_INFO::AnalyseExportTable();
    DD_INFO::AnalyseImportTable();
    DD_INFO::AnalyseBaseRelocationTable();
    return true;

    */
    return true;
}

LRESULT CALLBACK SectionTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
    static int cxClient, cyClient, cxChar, cyChar;
    HDC hdc;
    string tmp[4] ;
    PAINTSTRUCT ps;
    TCHAR szBuffer[256];
    TCHAR const* SecInfoForm[3] = {
            TEXT("Name:%s         Physical Address:%s         Virtual Address:%s         Size Of Raw Data:%s         Pointer To Raw Data%s"),
            TEXT("File Start:%s        File End:%s        Virtual Start:%s        Virtual End:%s"),
            TEXT("Characteristics:%s"),
    };
    size_t length;

    switch(message){
        case WM_SIZE:
            cxClient = GET_X_LPARAM(lparam);
            cyClient = GET_Y_LPARAM(lparam);
            return 0;
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            return 0;
        case WM_VSCROLL:

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            for (auto i = 0; i < NOS; i++) {
                StringCchPrintf(szBuffer, 256, SecInfoForm[0], secInfo[i][0].c_str(), secInfo[i][1].c_str(), secInfo[i][2].c_str(), secInfo[i][3].c_str(), secInfo[i][4].c_str());
                StringCchLength(szBuffer, 256, &length);
                TextOutA(hdc, 5 * cxChar, 2 * cyChar + cyClient * i / 6, szBuffer, int(length));

                StringCchPrintf(szBuffer, 256, SecInfoForm[1], secInfo[i][5].c_str(), secInfo[i][6].c_str(), secInfo[i][7].c_str(), secInfo[i][8].c_str());
                StringCchLength(szBuffer, 256, &length);
                TextOutA(hdc, 5 * cxChar, 7 * cyChar / 2 + cyClient * i / 6, szBuffer, int(length));

                StringCchPrintf(szBuffer, 256, SecInfoForm[2], secInfo[i][9].c_str());
                StringCchLength(szBuffer, 256, &length);
                TextOutA(hdc, 5 * cxChar, 5 * cyChar + cyClient * i / 6, szBuffer, int(length));
            }
            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, message, wparam, lparam);
}

LRESULT CALLBACK ExportTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
    return DefWindowProcA(hwnd, message, wparam, lparam);
}

LRESULT CALLBACK ImportTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
    return DefWindowProcA(hwnd, message, wparam, lparam);
}