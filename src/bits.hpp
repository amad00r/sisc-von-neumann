#pragma once

#include <bitset>
#include <cmath>

using namespace std;

template <typename T>
void copy_bits(const T bits, int start_pos, bitset<16>& word) {
    int bits_size = bits.size();
    for (int i = 0; i < bits_size; ++i)
        word[start_pos + i] = bits[i];
}

template <typename T>
void get_bits(T& bits, int start_pos, const bitset<16>& word) {
    int bits_size = bits.size();
    for (int i = 0; i < bits_size; ++i)
        bits[i] = word[start_pos + i];
}

bitset<8> hi_byte(const bitset<16>& word) {
    bitset<8> byte;
    for (int i = 0; i < 8; ++i)
        byte[i] = word[i + 8];
    return byte;
}

bitset<8> lo_byte(const bitset<16>& word) {
    bitset<8> byte;
    for (int i = 0; i < 8; ++i)
        byte[i] = word[i];
    return byte;
}

bitset<5> lo_bits5(const bitset<16>& word) {
    bitset<5> bits5;
    for (int i = 0; i < 5; ++i)
        bits5[i] = word[i];
    return bits5;
}

bitset<4> hi_bits4(const bitset<16>& word) {
    bitset<4> bits4;
    for (int i = 0; i < 4; ++i) {
        bits4[i] = word[i + 12];
    }
    return bits4;
}

char byte_to_char(const bitset<8> byte) {
    int result = 0;
    for (int i = 0; i < 8; ++i)
        result += pow(2, i)*byte[i];
    return result;
}

bitset<8> char_to_byte(const char c) {
    return bitset<8>(int(c));
}

template <typename T>
int bits_to_int(const T bits, bool ca2) {
    int size = bits.size();
    int result = pow(2, size - 1)*bits[size - 1];
    if (ca2) result = -result;
    for (int i = 0; i < size - 1; ++i) result += pow(2, i)*bits[i];
    return result;
}

template <typename T>
bitset<16> sign_extension(const T& bits) {
    bitset<16> word;
    copy_bits(bits, 0, word);
    if (bits[bits.size() - 1])
        for (int i = bits.size(); i < 16; ++i)
            word[i] = true;
    return word;
}