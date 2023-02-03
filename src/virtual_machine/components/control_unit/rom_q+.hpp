#include <bitset>

#include "states.hpp"

using namespace std;

int next_state(int state, bitset<4> instr_operation_code, bool operation_code_extension) {
    if (state == FETCH) return DECODE;
    if (state == ADDR) {
        if (instr_operation_code == 0b0011) return LD;
        if (instr_operation_code == 0b0100) return ST;
        if (instr_operation_code == 0b0101) return LDB;
        if (instr_operation_code == 0b0110) return STB;
    }
    if (state == DECODE) {
        if (instr_operation_code == 0b0000) return AL;
        if (instr_operation_code == 0b0001) return CMP;
        if (instr_operation_code == 0b0010) return ADDI;
        if (
            instr_operation_code == 0b0011 or
            instr_operation_code == 0b0100 or
            instr_operation_code == 0b0101 or
            instr_operation_code == 0b0110
        ) return ADDR;
        if (instr_operation_code == 0b0111) return JALR;
        if (instr_operation_code == 0b1000) {
            if (operation_code_extension) return BNZ;
            return BZ;
        }
        if (instr_operation_code == 0b1001) {
            if (operation_code_extension) return MOVHI;
            return MOVI;
        }
        if (instr_operation_code == 0b1010) {
            if (operation_code_extension) return OUT;
            return IN;
        }
        if (
            instr_operation_code == 0b1011 or
            instr_operation_code == 0b1100 or
            instr_operation_code == 0b1101 or
            instr_operation_code == 0b1110 or
            instr_operation_code == 0b1111
        ) return NOP;
    }
    if (state >= AL and state <= NOP and state != ADDR)
        return FETCH;

    throw runtime_error("Undefined behaviour");    
}