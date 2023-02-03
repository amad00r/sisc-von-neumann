#include <regex>
#include <string>
#include <bitset>
#include <cmath>

#include "../../exceptions/constant/CapacityExceededError.hpp"
#include "../../exceptions/constant/ConstantParsingError.hpp"
#include "../../exceptions/compile_error/RegisterError.hpp"

using namespace std;

bitset<3> get_register_id(const string& s) {
    smatch capture;
    regex r(R"(R([0-7]))");
    if (not regex_match(s, capture, r)) throw RegisterError(s);
    return bitset<3>(stoi(capture[1]));
}

bitset<6> get_6bits_constant(const string& s) {
    regex r(R"(-?\d+)");
    if (regex_match(s, r)) {
        int value = stoi(s, 0, 10);
        if (value < -32 or value > 31)
            throw CapacityExceededError(6, s);
        return bitset<6>(value);
    }

    smatch m;
    r = R"(0x([0-9a-fA-F]+))";
    if (regex_match(s, m, r)) {
        int value = stoi(m[1], 0, 16);
        if (value > 63)
            throw CapacityExceededError(6, s);
        return bitset<6>(value);
    }

    throw ConstantParsingError(s);
}

bitset<8> get_8bits_constant(const string& s) {
    regex r(R"(-?\d+)");
    if (regex_match(s, r)) {
        int value = stoi(s, 0, 10);
        if (value < -128 or value > 127)
            throw CapacityExceededError(8, s);
        return bitset<8>(value);
    }

    smatch m;
    r = R"(0x([0-9a-fA-F]+))";
    if (regex_match(s, m, r)) {
        int value = stoi(m[1], 0, 16);
        if (value > 255)
            throw CapacityExceededError(8, s);
        return bitset<8>(value);
    }

    throw ConstantParsingError(s);
}