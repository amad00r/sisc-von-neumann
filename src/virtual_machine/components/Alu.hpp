#include <bitset>

#include "../../bits.hpp"

using namespace std;

class Alu {
    private:
    bitset<16> sha(const bitset<16>& X, const bitset<16>& Y) {
        int offset = bits_to_int(lo_bits5(Y), true);
        if (offset < 0) {
            if (X[15]) {
                bitset<16> b = X;
                for (int i = 0; i < -offset; ++i) {
                    b >>= 1;
                    b[15] = true;
                }
                return b;
            }
            return X>>-offset;
        }
        return X<<offset;
    }
    bitset<16> shl(const bitset<16>& X, const bitset<16>& Y) {
        int offset = bits_to_int(lo_bits5(Y), true);
        if (offset < 0) return X>>-offset;
        return X<<offset;
    }
    bitset<16> cmplt(const bitset<16>& X, const bitset<16>& Y) {
        if (bits_to_int(X, true) < bits_to_int(Y, true))
            return bitset<16>(0b0000000000000001);
        return bitset<16>(0b0000000000000000);
    }
    bitset<16> cmple(const bitset<16>& X, const bitset<16>& Y) {
        if (bits_to_int(X, true) <= bits_to_int(Y, true))
            return bitset<16>(0b0000000000000001);
        return bitset<16>(0b0000000000000000);
    }
    bitset<16> cmpeq(const bitset<16>& X, const bitset<16>& Y) {
        if (X == Y)
            return bitset<16>(0b0000000000000001);
        return bitset<16>(0b0000000000000000);
    }
    bitset<16> cmpltu(const bitset<16>& X, const bitset<16>& Y) {
        if (bits_to_int(X, false) < bits_to_int(Y, false))
            return bitset<16>(0b0000000000000001);
        return bitset<16>(0b0000000000000000);
    }
    bitset<16> cmpleu(const bitset<16>& X, const bitset<16>& Y) {
        if (bits_to_int(X, false) <= bits_to_int(Y, false))
            return bitset<16>(0b0000000000000001);
        return bitset<16>(0b0000000000000000);
    }
    bitset<16> movhi(const bitset<16>& X, const bitset<16>& Y) {
        bitset<16> W = X;
        copy_bits(lo_byte(Y), 8, W);
        return W;
    }

    public:
    bool z = false;
    bitset<16> W;

    void run(const bitset<16>& X, const bitset<16>& Y, const bitset<2>& Op, const bitset<3>& F) {
        if (Op == 0b00) {
            if (F == 0b000) W = X&Y;
            else if (F == 0b001) W = X|Y;
            else if (F == 0b010) W = X^Y;
            else if (F == 0b011) W = ~X;
            else if (F == 0b100) W = bitset<16>(X.to_ulong() + Y.to_ulong());
            else if (F == 0b101) W = bitset<16>(X.to_ulong() - Y.to_ulong());
            else if (F == 0b110) W = sha(X, Y);
            else if (F == 0b111) W = shl(X, Y);
        }
        else if (Op == 0b01) {
            if (F == 0b000) W = cmplt(X, Y);
            else if (F == 0b001) W = cmple(X, Y);
            else if (F == 0b011) W = (X == Y);
            else if (F == 0b100) W = cmpltu(X, Y);
            else if (F == 0b101) W = cmpleu(X, Y);
            else W.reset();
        }
        else if (Op == 0b10) {
            if (F == 0b000) W = X;
            else if (F == 0b001) W = Y;
            else if (F == 0b010) W = movhi(X, Y);
            else if (F == 0b011) W = X&bitset<16>(0b1111111111111110);
            else W.reset();
        }
        else
            W.reset();

        z = W.none();
    }
};
