cmake_minimum_required(VERSION 3.20)
project(PEInfo)

set(CMAKE_CXX_STANDARD 14)

add_executable(PEInfo main.cpp NT_info.cpp NT_info.h DataDirectory_Info.cpp DataDirectory_Info.h FileLoader.cpp FileLoader.h DOS_info.cpp DOS_info.h File_header.cpp File_header.h Section_info.cpp Section_info.h Format.cpp Format.h OptionalHeader.cpp OptionalHeader.h DataDirectory.cpp DataDirectory.h)
