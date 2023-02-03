#include <string>
#include <iostream>

using namespace std;

class ActionError : public exception {
    private:
    string action;

    public:
    ActionError(const string& s) {
        action = s;
    }

    void explain() {
        cerr << "ActionError: `" << action << "` isn't a defined action" << endl << endl
             << "This is the list of defined actions:" << endl
             << "   `build`  : takes one argument (path) to a .sisa source code and generates a .bin binary of 16 bit machine code for the SISC Von Neumann" << endl
             << "   `run`    : takes one argument (path) to a .sisa or .bin file and executes an instance of the SISC Von Neumann to run the program" << endl
             << "   `readbin`: takes one argument (path) to a .bin file and outputs its contents in readable 16 bits binary instructions" << endl
             << "   `readhex`: takes one argument (path) to a .bin file and outputs its contents in readable 16 bits hexadecimal instructions" << endl;
    }
};