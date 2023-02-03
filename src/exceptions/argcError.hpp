#include <iostream>

using namespace std;

class argcError : public exception {
    private:
    int nargs;

    public:
    argcError(int n) {
        nargs = n;
    }

    void explain() {
        if (nargs > 3) cerr << "argcError: too many arguments, expected 3, got " << nargs << endl;
        else cerr << "argcError: too few arguments, expected 3, got " << nargs << endl; 
    }
};