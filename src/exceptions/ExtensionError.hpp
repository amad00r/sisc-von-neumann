#include <string>
#include <iostream>
#include <filesystem>

using namespace std;

class ExtensionError : public exception {
    private:
    string msg, extension;

    public:
    ExtensionError(const string& s, const filesystem::path& ext) {
        msg = s;
        extension = ext.string();
    }

    void explain() {
        cerr << "ExtensionError: " << msg << ", instead of a " << extension << " file" << endl;
    }
};