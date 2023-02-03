#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <bitset>
#include <iostream>

#include "bits.hpp"

#include "exceptions/IOError.hpp"

using namespace std;

typedef vector< bitset<16> > MachineCode;

stringstream read_file(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (not file.is_open()) throw IOError("the file could not be opened", file_path);
    stringstream ss;
    string line;
    while (getline(file, line)) ss << line << endl;
    return ss;
}

void write_binary(const MachineCode& mc, const filesystem::path& file_path) {
    ofstream file(file_path);
    if (not file.is_open()) throw IOError("couldn't open the file", file_path);
    int size = mc.size();
    for (int i = 0; i < size; ++i) {
        file.put(byte_to_char(hi_byte(mc[i])));
        file.put(byte_to_char(lo_byte(mc[i])));
    }
}

MachineCode read_binary(const filesystem::path& file_path) {
    ifstream file(file_path);
    if (not file.is_open()) throw IOError("couldn't open the file", file_path);

    MachineCode mc;
    char c;
    while (file.get(c)) {
        bitset<16> word;
        copy_bits(char_to_byte(c), 8, word);
        file.get(c);
        copy_bits(char_to_byte(c), 0, word);
        mc.push_back(word);
    }

    return mc;
}

void print_binary_content(const filesystem::path& file_path, bool bin_hex) {
    ifstream file(file_path);
    if (not file.is_open()) throw IOError("couldn't open the file", file_path);
    if (bin_hex) {
        char c;
        while (file.get(c)) {
            cout << char_to_byte(c).to_string();
            file.get(c);
            cout << char_to_byte(c).to_string() << endl;
        }
    }
    else {
        char c;
        while (file.get(c)) {
            cout << setfill('0') << setw(2) << hex << uppercase << char_to_byte(c).to_ulong();
            file.get(c);
            cout << setfill('0') << setw(2) << hex << uppercase << char_to_byte(c).to_ulong() << endl;
        }
    }
}