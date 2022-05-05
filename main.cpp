#include <bits/stdc++.h>

/************** sdk **************/
#include "windows.h"
#include "windowsx.h"
#include "strsafe.h"
#include "commdlg.h"

#include "Analyze.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

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
    PAINTSTRUCT ps;
    const int NUM = 10;
    static HWND hwndButtons[NUM];
    TCHAR szBuffer[128];
    size_t length;
    int cnt = 1000;
    string a = "i-386";
    static int cxClient, cyClient, cxChar, cyChar;
    static int id;
    TCHAR const *Infos[NUM] = {TEXT("FIleName :"),
                               TEXT("machine : "),
                               TEXT("SectionNumber : "),
                               TEXT("TimeDataStamp : "),
                               TEXT("SizeOfOptionalHeader : "),
                               TEXT("Characteristics : "),
                               TEXT("FileAlignment : "),
                               TEXT("SectionAlignment : "),
                               TEXT("ImageBase : "),
                               TEXT("AddressOfEntrypoint : "),
                               };

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
            return 0;
        case WM_CLOSE:
            if (MessageBox(hwnd, TEXT("please click yes if you want to quit"), TEXT("demo"), MB_OKCANCEL) == IDOK){
                DestroyWindow(hwnd);
            } else{
                return 0;
            }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, message, wparam, lparam);
}