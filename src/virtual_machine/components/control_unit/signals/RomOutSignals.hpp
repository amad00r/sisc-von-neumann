#pragma once

#include <bitset>

using namespace std;

struct RomOutSignals {
    bool Bnz;
    bool Bz;
    bool WrMem;
    bool RdIn;
    bool WrOut;
    bool Wrd;
    bool LdIr;
    bool Byte;
    bool RPc;
    bool AluR;
    bool PcRx;
    bool RyN;
    bitset<2> Pila;
    bitset<2> Op;
    bitset<2> MxN;
    bool MxF;
    bitset<3> F;
    bitset<2> MxD;
};