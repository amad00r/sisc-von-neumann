#include <bitset>

#include "signals/RomOutSignals.hpp"
#include "states.hpp"

using namespace std;

RomOutSignals rom_out_signals(int state) {
    RomOutSignals signals;

    signals.Bnz = (state == FETCH or state == JALR or state == BNZ);
    signals.Bz = (state == FETCH or state == JALR or state == BZ);

    signals.WrMem = (state == ST or state == STB);
    signals.RdIn = (state == IN);
    signals.WrOut = (state == OUT);

    signals.Wrd = (
        state == AL or 
        state == CMP or 
        state == ADDI or 
        state == LD or 
        state == LDB or 
        state == JALR or 
        state == MOVI or 
        state == MOVHI or 
        state == IN
    );

    signals.LdIr = (state == FETCH);

    signals.Byte = (state == LDB or state == STB);

    signals.RPc = (state >= LD and state <= STB);
    signals.AluR = (state == FETCH or state == JALR);
    signals.PcRx = (state == FETCH or state == DECODE);
    signals.RyN = (state == AL or state == CMP);

    signals.Pila[0] = (state == LD or state == LDB or state == JALR);
    signals.Pila[1] = (state == JALR or state == IN);

    signals.Op[0] = (state == CMP);
    signals.Op[1] = (state >= JALR and state <= MOVHI);

    signals.MxN[0] = (state == FETCH or state == MOVI or state == MOVHI);
    signals.MxN[1] = (state == FETCH or state == DECODE);

    signals.MxF = (
        state == FETCH or
        state == DECODE or
        state == ADDI or
        state == ADDR or
        (state >= JALR and state <= MOVHI)
    );

    signals.F[0] = (state == JALR or state == MOVI);
    signals.F[1] = (state == JALR or state == MOVHI);
    signals.F[2] = (
        state == FETCH or 
        state == DECODE or 
        state == ADDI or 
        state == ADDR
    );

    signals.MxD[0] = (
        state == ADDI or 
        state == LD or 
        state == LDB or 
        state == JALR
    );
    signals.MxD[1] = (state == MOVI or state == MOVHI or state == IN);
    
    return signals;
}