#pragma once

#include <string>
#include <iostream>

using namespace std;

class ConstantParsingError : public exception {
    private:
    string constant;

    public:
    ConstantParsingError(const string& s) {
        constant = s;
    }

    void explain() {
        cerr << "ConstantParsingError: constant `" << constant
             << "` cannot be parsed due bad syntax" << endl
             << R"(  decimal constants must follow the pattern: regex("-?\d+"))" << endl
             << "  hexadecimal constants must follow the pattern: regex(\"0x([0-9a-fA-F]+)\")" << endl;
    }
};