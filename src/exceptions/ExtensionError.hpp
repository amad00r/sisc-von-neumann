#include <string>
#include <iostream>

using namespace std;

class ExtensionError : public exception {
    private:
    string msg, extension;

    public:
    ExtensionError(const string& s, const string& ext) {
        msg = s;
        extension = ext;
    }

    void explain() {
        cerr << "ExtensionError: " << msg << ", instead of a " << extension << " file" << endl;
    }
};