#ifndef PEINFO_ANALYZE_H
#define PEINFO_ANALYZE_H

#include "FileLoader.h"
#include "DOS_info.h"
#include "NT_info.h"
#include "File_header.h"
#include "OptionalHeader.h"
#include "Section_info.h"
#include "DataDirectory.h"
#include "Format.h"
#include "windowsx.h"
#include "strsafe.h"

LRESULT CALLBACK SectionTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ExportTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ImportTableProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

class Analyze {
public:
    static bool Ana(HWND hwnd, char* dst);
};


#endif //PEINFO_ANALYZE_H
