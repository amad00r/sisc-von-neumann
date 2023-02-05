#pragma once

#include <string>

const int FETCH  = 0;
const int DECODE = 1;
const int AL     = 2;
const int CMP    = 3;
const int ADDI   = 4;
const int ADDR   = 5;
const int LD     = 6;
const int ST     = 7;
const int LDB    = 8;
const int STB    = 9;
const int JALR   = 10;
const int BZ     = 11;
const int BNZ    = 12;
const int MOVI   = 13;
const int MOVHI  = 14;
const int IN     = 15;
const int OUT    = 16;
const int NOP    = 17;

string state_to_string(int state) {
    if (state == FETCH) return "FETCH";
    if (state == DECODE) return "DECODE";
    if (state == AL) return "AL";
    if (state == CMP) return "CMP";
    if (state == ADDI) return "ADDI";
    if (state == ADDR) return "ADDR";
    if (state == LD) return "LD";
    if (state == ST) return "ST";
    if (state == LDB) return "LDB";
    if (state == STB) return "STB";
    if (state == JALR) return "JALR";
    if (state == BZ) return "BZ";
    if (state == BNZ) return "BNZ";
    if (state == MOVI) return "MOVI";
    if (state == MOVHI) return "MOVHI";
    if (state == IN) return "IN";
    if (state == OUT) return "OUT";
    if (state == NOP) return "NOP";
    throw "unexpected error";
}