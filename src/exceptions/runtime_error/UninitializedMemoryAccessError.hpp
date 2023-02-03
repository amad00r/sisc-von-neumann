#pragma once

#include <string>
#include <iostream>
#include <bitset>

using namespace std;

class UninitializedMemoryAccessError : public exception {
    private:
    string word_byte;
    bitset<16> mem_address;

    public:
    UninitializedMemoryAccessError(const string& read_mode, const bitset<16>& addr) {
        word_byte = read_mode;
        mem_address = addr;
    }

    void explain() {
        cerr << "UninitializedMemoryAccessError: access to an uninitialized " << word_byte
             << " stored in (" << mem_address.to_string() << ") was attempted" << endl;
    }
};