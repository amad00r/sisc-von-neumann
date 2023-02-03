#include <string>
#include <iostream>

using namespace std;

class IOError : public exception {
    private:
    string msg, file_path;

    public:
    IOError(const string& s, const string& path) {
        msg = s;
        file_path = path;
    }

    void explain() {
        cerr << "IOError: " << msg << ": " << file_path << endl;
    }
};