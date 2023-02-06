#include <bitset>
#include <iostream>
#include <regex>
#include <string>

#include "../../exceptions/constant/CapacityExceededError.hpp"
#include "../../exceptions/constant/ConstantParsingError.hpp"
#include "../../exceptions/runtime_error/InvalidInputError.hpp"
#include "../../exceptions/runtime_error/UnauthorizedDataPortAccessError.hpp"

using namespace std;

class InputOutput {
    private:
    bitset<16> key_data = bitset<16>(0);
    bool key_status = false;
    bool print_status = false;

    bitset<16> input_data_value(const string& s) {
        regex r(R"(-?\d+)");
        if (regex_match(s, r)) {
            int value = stoi(s, 0, 10);
            if (value < -32768 or value > 32767)
                throw CapacityExceededError(16, s);
            return bitset<16>(value);
        }

        smatch m;
        r = R"(0x([0-9a-fA-F]+))";
        if (regex_match(s, m, r)) {
            int value = stoi(m[1], 0, 16);
            if (value > 65535)
                throw CapacityExceededError(16, s);
            return bitset<16>(value);
        }

        throw ConstantParsingError(s);
    }

    bool input_status_value(const string& s) {
        if (s == "0") return false;
        if (s == "1") return true;
        throw InvalidInputError(s);
    }

    void ask_key_status() {
        cout << "Key-Req = ";
        string input;
        getline(cin, input);
        key_status = input_status_value(input);
    }

    void ask_key_data() {
        cout << "KEY-DATA = ";
        string input;
        getline(cin, input);
        key_data = input_data_value(input);
    }

    public:
    bitset<16> rd_in;

    void run(const bitset<8>& AddrIo, const bitset<16>& wr_out, bool WrOut, bool RdIn) {
        if (not AddrIo[0] and not AddrIo[1]) {
            if (RdIn) {
                if (not key_status) throw UnauthorizedDataPortAccessError(true);
                ask_key_data();
                key_status = false;
            }
            rd_in = key_data;
            if (WrOut) {
                if (not print_status) throw UnauthorizedDataPortAccessError(false);
                cout << wr_out.to_string() << endl;
                print_status = false;
            }
        }
        else if (AddrIo[0] and not AddrIo[1]) {
            ask_key_status();
            if (key_status) rd_in = bitset<16>(1);
            else rd_in = bitset<16>(0);
        }
        else if (AddrIo[1]) {
            print_status = true;
            rd_in = bitset<16>(1);
        }
    }
};