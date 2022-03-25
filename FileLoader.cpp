#include "FileLoader.h"

HANDLE FileLoader::hMapping     = nullptr;
HANDLE FileLoader::hFile        = nullptr;
LPVOID FileLoader::ImageBase    = nullptr;

int FileLoader::Get_hFile(char* file) {
    hFile     = CreateFile(file, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == nullptr){
        return OPENFILESUCCESS | ERRORCODE;
    }
    return OPENFILESUCCESS;
}

int FileLoader::Get_hMapping() {
    hMapping  = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (hMapping == nullptr){
        return CREATEMAPPINGSUCCESS | ERRORCODE;
    }
    return CREATEMAPPINGSUCCESS;
}

int FileLoader::Get_ImageBase() {
    ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ,0,0,0);
    if (hMapping == nullptr){
        return MAPPINGSPACESUCCESS | ERRORCODE;
    }
    return MAPPINGSPACESUCCESS;
}