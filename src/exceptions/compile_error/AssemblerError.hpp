#pragma once

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class AssemblerError : public exception {
    private:
    int line_number;
    string instruction; 
    T* specific_error;

    public:
    AssemblerError(int n, const string& instr, T& error) {
        line_number = n;
        instruction = instr;
        specific_error = &error;
    }

    void explain() {
        cerr << "AssemblerError: failed to assemble due to an error in line " << line_number << ": " << instruction << endl;
        specific_error->explain();
    }
};