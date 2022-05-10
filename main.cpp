#include <bits/stdc++.h>

/************** sdk **************/
#include "windows.h"
#include "commdlg.h"
#include "tchar.h"

#include "Analyze.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);



bool ChooseFile(HWND hwnd);
bool CreateWndClass(WNDCLASS* wndclass);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR iCmdLine, int iCmdShow){
    HWND hwnd;
    WNDCLASS wndclass;
    MSG msg;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor (nullptr, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = TEXT("PEInfo");
    wndclass.lpszMenuName = nullptr;

    if (!RegisterClassA(&wndclass)){
        MessageBox(nullptr, "error: can't register wndclass", "error box", MB_OKCANCEL);
    }

    hwnd = CreateWindow(TEXT("PEInfo"), TEXT("PEInfo"), WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, nullptr, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
    HDC hdc;
    HWND childHwnd;
    WNDCLASS childWndClass[3];
    MSG msg;
    PAINTSTRUCT ps;
    HINSTANCE hChildInstance;
    char* fileName;
    const int NUM = 10;
    const int DLG_SECTION = 2001;
    static HWND hwndButtons[NUM];
    TCHAR szBuffer[128];
    size_t length;

    int cnt = 1000;
    static int cxClient = 0, cyClient = 0, cxChar = 0, cyChar = 0;
    static int id = 0;
    TCHAR const *Infos[NUM] = {
            TEXT("FileName :"),
            TEXT("machine : "),
            TEXT("SectionNumber : "),
            TEXT("TimeDataStamp : "),
            TEXT("SizeOfOptionalHeader : "),
            TEXT("Characteristics : "),
            TEXT("FileAlignment : "),
            TEXT("SectionAlignment : "),
            TEXT("ImageBase : "),
            TEXT("AddressOfEntrypoint : ")
    };
    TCHAR const *AnInfos[3] = {
            TEXT("Section Table"),
            TEXT("Export Table"),
            TEXT("Import Table")
    };

    CreateWndClass(&childWndClass[0]);
    CreateWndClass(&childWndClass[1]);
    CreateWndClass(&childWndClass[2]);

    switch (message) {
        case WM_SIZE:
            cxClient = GET_X_LPARAM(lparam);
            cyClient = GET_Y_LPARAM(lparam);
            return 0;
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            CreateWindow(TEXT("button"), TEXT("PE Base Info"), BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
                         cxChar, cyChar/2,
                         135 * cxChar, 29 * cyChar,
                         hwnd, reinterpret_cast<HMENU>(1000),
                         ((LPCREATESTRUCT)lparam)->hInstance, nullptr);

            CreateWindow(TEXT("button"), TEXT("Extra Info"), BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
                         cxChar, 31 * cyChar,
                         135 * cxChar, 5 * cyChar,
                         hwnd, reinterpret_cast<HMENU>(2000),
                         ((LPCREATESTRUCT)lparam)->hInstance, nullptr);

            hwndButtons[0] = CreateWindow(TEXT("button"), TEXT(" "), BS_BOTTOM|WS_VISIBLE|WS_CHILD|WS_BORDER,
                                          cxChar * 20 , cyChar * 13/ 5,
                                          cxChar * 100, cyChar * 7 / 4,
                                          hwnd, reinterpret_cast<HMENU>(1001),
                                          ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
            for (auto i = 1; i < 5; i++){
                hwndButtons[i] = CreateWindow(TEXT("button"), TEXT(" "), BS_BOTTOM|WS_VISIBLE|WS_CHILD|WS_BORDER,
                                              cxChar * (20 + 7 * cxChar * ((i+1) % 2)) , cyChar * (23 / 5 + 19 * ((i+1) / 2) / 4),
                                              cxChar * 30, cyChar * 7 / 4,
                                              hwnd, reinterpret_cast<HMENU>(1001 + i),
                                              ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
            }
            hwndButtons[5] = CreateWindow(TEXT("button"), TEXT(" "), BS_BOTTOM|WS_VISIBLE|WS_CHILD|WS_BORDER,
                                          cxChar * 20 , cyChar * ( 21 / 5 + 57 / 4),
                                          cxChar * 100, cyChar * 7 / 4,
                                          hwnd, reinterpret_cast<HMENU>(1006),
                                          ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
            for (auto i = 6; i < 10; i++){
                hwndButtons[i] = CreateWindow(TEXT("button"), TEXT(" "), BS_BOTTOM|WS_VISIBLE|WS_CHILD|WS_BORDER,
                                              cxChar * (20 + 7 * cxChar * ((i+2) % 2)) , cyChar * (23 / 5 + 19 * ((i+2) / 2) / 4),
                                              cxChar * 30, cyChar * 7 / 4,
                                              hwnd, reinterpret_cast<HMENU>(1001 + i),
                                              ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
            }

            for (auto j = 1; j < 4; j++){
                hwndButtons[j] = CreateWindow(TEXT("button"), AnInfos[j-1], BS_BOTTOM|WS_VISIBLE|WS_CHILD|WS_BORDER,
                                              cxChar * (6 + 45 * (j - 1)), cyChar * 33,
                                              cxChar * 35, cyChar * 2,
                                              hwnd, reinterpret_cast<HMENU>(2000 + j),
                                              ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
            }

            childWndClass[0].hInstance = hChildInstance;
            childWndClass[0].lpfnWndProc = SectionTableProc;
            childWndClass[0].lpszClassName = TEXT("SectionInfo");
            childWndClass[1].hInstance = hChildInstance;
            childWndClass[1].lpfnWndProc = ExportTableProc;
            childWndClass[1].lpszClassName = TEXT("ExportInfo");
            childWndClass[2].hInstance = hChildInstance;
            childWndClass[2].lpfnWndProc = ImportTableProc;
            childWndClass[2].lpszClassName = TEXT("ImportInfo");
            if (!RegisterClass(&childWndClass[0]) || !RegisterClass(&childWndClass[1]) || !RegisterClass(&childWndClass[2])){
                MessageBox(nullptr, TEXT("create child window failed"), TEXT("title"), MB_ICONERROR);
                return 0;
            }

            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            StringCchPrintf(szBuffer, 128, TEXT("%s"), Infos[0]);
            StringCchLength(szBuffer, 128, &length);
            TextOutA(hdc, cxChar * 5, cyChar * 3, szBuffer, int(length));
            for (auto i = 1; i < 5; i++){
                StringCchPrintf(szBuffer, 128, TEXT("%s"), Infos[i]);
                StringCchLength(szBuffer, 128, &length);
                TextOutA(hdc, cxChar * (5 + 50 * ((i+1) % 2)), cyChar * 5 * ((i + 1) / 2) + cyChar * 3, szBuffer, int(length));
            }
            StringCchPrintf(szBuffer, 128, TEXT("%s"), Infos[5]);
            StringCchLength(szBuffer, 128, &length);
            TextOutA(hdc, cxChar * 5, cyChar * 18, szBuffer, int(length));
            for (auto i = 6; i < 10; i++){
                StringCchPrintf(szBuffer, 128, TEXT("%s"), Infos[i]);
                StringCchLength(szBuffer, 128, &length);
                TextOutA(hdc, cxChar * (5 + 50 * ((i+2) % 2)), cyChar * 4 * ((i + 2) / 2) + cyChar * 15 / 2, szBuffer, int(length));
            }

            EndPaint(hwnd, &ps);
            return 0;

        case WM_COMMAND:
            id = LOWORD(wparam);
            switch (id) {
                case 1001:
                    for (auto i = 1; i < 11; i++){
                        SendDlgItemMessage(hwnd, i+1000, WM_SETTEXT,MAX_PATH,NULL);
                    }
                    if (!ChooseFile(hwnd)){
                        MessageBox(hwnd, TEXT("can't open file"), TEXT("ERROR BOX"), MB_OK);
                        return 0;
                    }
                    break;
                case 2001:
                    childHwnd = CreateWindow(TEXT("SectionInfo"), TEXT("SectionInfo"), WS_OVERLAPPEDWINDOW | WS_VSCROLL,
                                        CW_USEDEFAULT, CW_USEDEFAULT,
                                        CW_USEDEFAULT, CW_USEDEFAULT,
                                        nullptr, nullptr, hChildInstance, nullptr);
                    ShowWindow(childHwnd, SW_SHOW);
                    UpdateWindow(childHwnd);
                    while(GetMessage(&msg, nullptr, 0, 0)){
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    break;
                case 2002:
                    childHwnd = CreateWindow(TEXT("ExportInfo"), TEXT("ExportInfo"), WS_OVERLAPPEDWINDOW,
                                             CW_USEDEFAULT, CW_USEDEFAULT,
                                             CW_USEDEFAULT, CW_USEDEFAULT,
                                             nullptr, nullptr, hChildInstance, nullptr);
                    ShowWindow(childHwnd, SW_SHOW);
                    UpdateWindow(childHwnd);
                    while(GetMessage(&msg, nullptr, 0, 0)){
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    break;
                case 2003:
                    childHwnd = CreateWindow(TEXT("ImportInfo"), TEXT("ImportInfo"), WS_OVERLAPPEDWINDOW,
                                             CW_USEDEFAULT, CW_USEDEFAULT,
                                             CW_USEDEFAULT, CW_USEDEFAULT,
                                             nullptr, nullptr, hChildInstance, nullptr);
                    ShowWindow(childHwnd, SW_SHOW);
                    UpdateWindow(childHwnd);
                    while(GetMessage(&msg, nullptr, 0, 0)){
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    break;
            }
            return 0;
        case WM_CLOSE:
            if (MessageBox(hwnd, TEXT("please click yes if you want to quit"), TEXT("close box"), MB_OKCANCEL) == IDOK){
                DestroyWindow(hwnd);
            } else{
                return 0;
            }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            break;
    }
    return DefWindowProcA(hwnd, message, wparam, lparam);
}

bool ChooseFile(HWND hwnd){
    OPENFILENAME openfilename;
    TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
    TCHAR szPath[MAX_PATH];
    TCHAR szFileName[80 * MAX_PATH];

    int nLen;
    TCHAR* p;
    ZeroMemory(&openfilename, sizeof openfilename);

    openfilename.lStructSize = sizeof openfilename;
    openfilename.hwndOwner = nullptr;
    openfilename.lpstrFile = szOpenFileNames;
    openfilename.nMaxFile = sizeof szOpenFileNames;
    openfilename.lpstrFile[0] = _T('\0');
    openfilename.lpstrFilter = _T(".exe\0*.exe\0.dll\0*.dll");
    openfilename.nFilterIndex = 1;
    openfilename.lpstrTitle = _T("please choose a file");
    openfilename.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_EXPLORER;
    if (!::GetOpenFileName(&openfilename)){
        return false;
    }
    lstrcpyn(szPath, szOpenFileNames, openfilename.nFileOffset);
    szPath[openfilename.nFileOffset] = '\0';
    nLen = lstrlen(szPath);
    if (szPath[nLen - 1] != '\\'){
        StringCchCatA(szPath, strlen(szPath) + 4, _T("\\"));
    }
    p = szOpenFileNames + openfilename.nFileOffset;
    ZeroMemory(szFileName, sizeof szFileName);
    string str;
    string fileName = LPSTR(p);
    string filePath = LPSTR(szPath);
    str += filePath + fileName + '\0';
    char * ret = new char[strlen(str.c_str()) + 1];
    const char * cc = str.c_str();
    strcpy_s(ret, str.size()+1, cc);
    SetDlgItemText(hwnd, 1001, ret);
    Analyze::Ana(hwnd, ret);
    return true;
}

bool CreateWndClass(WNDCLASS* wndClass){
    wndClass->style = CS_HREDRAW | CS_VREDRAW;
    wndClass->cbClsExtra = 0;
    wndClass->cbWndExtra = 0;
    wndClass->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass->hCursor = LoadCursor (nullptr, IDC_ARROW);
    wndClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass->lpszMenuName = nullptr;
    return true;
}

