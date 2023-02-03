#pragma once

#include <string>
#include <iostream>

using namespace std;

class RegisterError : public exception {
    private:
    string reg;

    public:
    RegisterError(const string& s) {
        reg = s;
    }

    void explain() {
        cerr << "RegisterError: register `" << reg << "` doesn't name a valid register. Registers must follow the pattern: regex(\"R[0-7]\")" << endl;
    }
};