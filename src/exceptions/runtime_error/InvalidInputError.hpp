#pragma once

#include <string>
#include <iostream>

using namespace std;

class InvalidInputError : public exception {
    private:
    string input;

    public:
    InvalidInputError(const string& s) {
        input = s;
    }

    void explain() {
        cerr << "InvalidInputError: expected either \"1\" or \"0\" as input, instead of \"" << input << "\"" << endl;
    }
};