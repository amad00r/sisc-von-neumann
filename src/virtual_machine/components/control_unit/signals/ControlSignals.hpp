#pragma once

#include <bitset>

using namespace std;

struct ControlSignals {
    bitset<3> A;
    bitset<3> B;
    bool PcRx;
    bool RyN;
    bitset<2> Op;
    bitset<3> F;
    bitset<2> Pila;
    bitset<3> D;
    bool Wrd;
    bool WrOut;
    bool RdIn;
    bool WrMem;
    bool LdIr;
    bool LdPc;
    bool Byte;
    bool AluR;
    bool RPc;
    bitset<16> N;
    bitset<8> AddrIo;
};