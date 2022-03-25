#include "Format.h"

/************************* just for beauty *************************/
// this function is used to change a unsigned to hexx format like 0x00000000 or 0x0000

string FORMAT::WordToHex(WORD word) {
    char* test = new(char);
    sprintf(test, "%04X", word);
    return "0x" + string(test);
}

string FORMAT::DwordToHex(DWORD dword) {
    char* test = new(char);
    sprintf(test, "%08X", dword);
    return "0x" + string(test);
}