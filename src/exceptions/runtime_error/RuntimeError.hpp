#pragma once

#include <string>
#include <iostream>

using namespace std;

template <typename T>
class RuntimeError : public exception {
    private:
    bitset<16> instruction, mem_address;
    T* specific_error;

    public:
    RuntimeError(const bitset<16>& instr, const bitset<16>& addr, T& error) {
        instruction = instr;
        mem_address = addr;
        specific_error = &error;
    }

    void explain() {
        cerr << "RuntimeError: execution of the program was aborted while executing the instruction ("
             << instruction << ") stored in memory address (" << mem_address << ")" << endl;
        specific_error->explain();
    }
};