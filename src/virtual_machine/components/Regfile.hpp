#include <bitset>

using namespace std;

class Regfile {
    private:
    bitset<16> regfile[8];

    public:
    bitset<16> A = bitset<16>(0);
    bitset<16> B = bitset<16>(0);

    void run(const bitset<3>& atA, const bitset<3>& atB, const bitset<3>& atD, const bitset<16> D, bool Wrd) {
        A = regfile[atA.to_ulong()];
        B = regfile[atB.to_ulong()];
        if (Wrd) regfile[atD.to_ulong()] = D;
    }
};