#include <bitset>

#include "../../../bits.hpp"
#include "signals/ControlSignals.hpp"
#include "signals/RomOutSignals.hpp"
#include "rom_q+.hpp"
#include "rom_out.hpp"

using namespace std;

class ControlUnit {
    private:
    int state = FETCH;
    RomOutSignals signals_rom;

    public:
    int current_state;
    ControlSignals signals;

    void run(const bitset<16>& instruction) {
        current_state = state;

        signals_rom = rom_out_signals(state);

        signals.WrMem = signals_rom.WrMem;
        signals.RdIn = signals_rom.RdIn;
        signals.WrOut = signals_rom.WrOut;
        signals.Wrd = signals_rom.Wrd;
        signals.LdIr = signals_rom.LdIr;
        signals.Byte = signals_rom.Byte;
        signals.RPc = signals_rom.RPc;
        signals.AluR = signals_rom.AluR;
        signals.PcRx = signals_rom.PcRx;
        signals.RyN = signals_rom.RyN;
        signals.Pila = signals_rom.Pila;
        signals.Op = signals_rom.Op;

        get_bits(signals.A, 9, instruction);
        get_bits(signals.B, 6, instruction);

        if (signals_rom.MxD == bitset<2>(0b00)) get_bits(signals.D, 3, instruction);
        else if (signals_rom.MxD == bitset<2>(0b01)) get_bits(signals.D, 6, instruction);
        else if (signals_rom.MxD == bitset<2>(0b10) or signals_rom.MxD == bitset<2>(0b11))
            get_bits(signals.D, 9, instruction);

        if (signals_rom.MxF) signals.F = signals_rom.F;
        else get_bits(signals.F, 0, instruction);

        if (signals_rom.MxN == bitset<2>(0b00)) {
            bitset<6> N;
            get_bits(N, 0, instruction);
            signals.N = sign_extension(N);
        }
        else if (signals_rom.MxN == bitset<2>(0b01)) {
            bitset<8> N;
            get_bits(N, 0, instruction);
            signals.N = sign_extension(N);
        }
        else if (signals_rom.MxN == bitset<2>(0b10)) {
            bitset<8> N;
            get_bits(N, 0, instruction);
            signals.N = sign_extension(N)<<1;
        }
        else if (signals_rom.MxN == bitset<2>(0b11))
            signals.N = bitset<16>(0x0002);

        signals.AddrIo = lo_byte(instruction);

        state = next_state(state, hi_bits4(instruction), instruction[8]);
    }

    void calc_ldpc(bool z) {
        signals.LdPc = ((signals_rom.Bz and z) or (signals_rom.Bnz and not z));
    }
};