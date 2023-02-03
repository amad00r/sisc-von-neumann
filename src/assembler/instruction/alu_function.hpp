#include <regex>
#include <string>
#include <bitset>

using namespace std;

bitset<3> get_al_function(const string& s) {
    if (s == "AND") return bitset<3>(0b000);
    if (s == "OR") return bitset<3>(0b001);
    if (s == "XOR") return bitset<3>(0b010);
    if (s == "NOT") return bitset<3>(0b011);
    if (s == "ADD") return bitset<3>(0b100);
    if (s == "SUB") return bitset<3>(0b101);
    if (s == "SHA") return bitset<3>(0b110);
    if (s == "SHL") return bitset<3>(0b111);
    throw "unexpected error";
}

bitset<3> get_cmp_function(const string& s) {
    if (s == "CMPLT") return bitset<3>(0b000);
    if (s == "CMPLE") return bitset<3>(0b001);
    if (s == "CMPEQ") return bitset<3>(0b011);
    if (s == "CMPLTU") return bitset<3>(0b100);
    if (s == "CMPLEU") return bitset<3>(0b101);
    throw "unexpected error";
}