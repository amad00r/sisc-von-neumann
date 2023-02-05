#include <string>
#include <iostream>

using namespace std;

class IOError : public exception {
    private:
    string msg, file_path;

    public:
    IOError(const string& s, const filesystem::path& path) {
        msg = s;
        file_path = path.string();
    }

    void explain() {
        cerr << "IOError: " << msg << ": " << file_path << endl;
    }
};