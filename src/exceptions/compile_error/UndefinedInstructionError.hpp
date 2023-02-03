#pragma once

#include <string>
#include <iostream>
#include <regex>

using namespace std;

class UndefinedInstructionError : public exception {
    private:
    string instruction;

    public:
    UndefinedInstructionError(const string& s) {
        smatch capture;
        regex r(R"((\S+).*)");
        regex_match(s, capture, r);
        instruction = capture[1];
    }

    void explain() {
        cerr << "UndefinedInstructionError: instruction `" << instruction
             << "` is not part of the SISA set of instructions" << endl;
    }
};