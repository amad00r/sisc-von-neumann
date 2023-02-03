#pragma once

#include <iostream>
#include <bitset>

using namespace std;

class UnauthorizedDataPortAccessError : public exception {
    private:
    bool rd_wr;

    public:
    UnauthorizedDataPortAccessError(bool RdIn_WrOut) {
        rd_wr = RdIn_WrOut;
    }

    void explain() {
        if (rd_wr)
            cerr << "UnauthorizedDataPortAccessError: access to keyword data port 0 was attempted while status port 1 indicates 0" << endl
                 << "Make sure to check the status port before accessing the data port. You can use the following construction to do so:" << endl
                 << "   IN RX, 1" << endl
                 << "   BZ RX, -2" << endl;
        else
            cerr << "UnauthorizedDataPortAccessError: writing to printer data port 0 was attempted while status port 2 indicates 0" << endl
                 << "Make sure to check the status port before writing the data port. You can use the following construction to do so:" << endl
                 << "   IN RX, 2" << endl
                 << "   BZ RX, -2" << endl;
    }
};