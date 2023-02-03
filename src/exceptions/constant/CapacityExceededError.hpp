#pragma once

#include <string>
#include <iostream>

using namespace std;

class CapacityExceededError : public exception {
    private:
    int capacity;
    string constant;

    public:
    CapacityExceededError(int nbits, const string& s) {
        capacity = nbits;
        constant = s;
    }

    void explain() {
        cerr << "CapacityExceededError: constant `" << constant
             << "` value exceeds " << capacity << " bits capacity" << endl;
    }
};