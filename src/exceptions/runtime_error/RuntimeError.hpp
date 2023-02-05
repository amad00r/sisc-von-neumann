#pragma once

#include <string>
#include <iostream>

using namespace std;

template <typename T>
class RuntimeError : public exception {
    private:
    bitset<16> instruction, mem_address;
    string state;
    T* specific_error;

    public:
    RuntimeError(const bitset<16>& instr, const bitset<16>& addr, const string& s, T& error) {
        instruction = instr;
        mem_address = addr;
        state = s;
        specific_error = &error;
    }

    void explain() {
        cerr << "RuntimeError: execution of the program was aborted while state = " << state
             << ", IR = " << instruction.to_string() << ", PC = " << mem_address.to_string() << endl;
        specific_error->explain();
    }
};