#pragma once

#include <string>
#include <iostream>

using namespace std;

class ParametersError : public exception {
    public:
    void explain() {
        cerr << "ParametersError: instruction parameters cannot be parsed due to bad syntax" << endl;
    }
};