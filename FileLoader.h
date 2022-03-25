#include <windows.h>

#ifndef PEINFO_FILELOADER_H
#define PEINFO_FILELOADER_H

const int ERRORCODE             = 1 << 0;
const int OPENFILESUCCESS       = 1 << 1;
const int CREATEMAPPINGSUCCESS  = 1 << 2;
const int MAPPINGSPACESUCCESS   = 1 << 3;

class FileLoader{

public:
    static int Get_hFile(char*);
    static int Get_hMapping();
    static int Get_ImageBase();

    static LPVOID ImageBase;   // ImageBase

private:
    static HANDLE hFile;       // hwnd
    static HANDLE hMapping;    // reflect hwnd
};

#endif //PEINFO_FILELOADER_H
