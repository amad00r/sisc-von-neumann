#include <vector>
#include <bitset>

#include "components/Memory.hpp"
#include "components/Alu.hpp"
#include "components/Regfile.hpp"
#include "components/InputOutput.hpp"
#include "components/control_unit/ControlUnit.hpp"
#include "components/control_unit/states.hpp"

#include "../exceptions/runtime_error/RuntimeError.hpp"
#include "../exceptions/runtime_error/InvalidInputError.hpp"
#include "../exceptions/runtime_error/UnauthorizedDataPortAccessError.hpp"
#include "../exceptions/runtime_error/UninitializedMemoryAccessError.hpp"
#include "../exceptions/constant/CapacityExceededError.hpp"
#include "../exceptions/constant/ConstantParsingError.hpp"

using namespace std;

typedef vector< bitset<16> > MachineCode;

class Processor {
    private:
    Memory mem;
    Alu alu;
    Regfile regfile;
    InputOutput io;
    ControlUnit control_unit;

    bitset<16> rx = bitset<16>(0);
    bitset<16> ry = bitset<16>(0);
    bitset<16> ir = bitset<16>(0);
    bitset<16> r_at = bitset<16>(0);
    bitset<16> pc = bitset<16>(0);

    public:
    void load(const MachineCode& mc) {
        mem.allocate_program(mc);
    }

    void cicle() {
        try {
            control_unit.run(ir);

            if (control_unit.signals.PcRx and control_unit.signals.RyN)
                alu.run(pc, ry, control_unit.signals.Op, control_unit.signals.F);
            else if (not control_unit.signals.PcRx and control_unit.signals.RyN)
                alu.run(rx, ry, control_unit.signals.Op, control_unit.signals.F);
            else if (control_unit.signals.PcRx and not control_unit.signals.RyN)
                alu.run(pc, control_unit.signals.N, control_unit.signals.Op, control_unit.signals.F);
            else
                alu.run(rx, control_unit.signals.N, control_unit.signals.Op, control_unit.signals.F);
            
            control_unit.calc_ldpc(alu.z);

            bool need_to_load = control_unit.signals.LdIr or control_unit.signals.Pila == bitset<2>(0b01);
            if (control_unit.signals.RPc)
                mem.run(r_at, ry, control_unit.signals.WrMem, control_unit.signals.Byte, need_to_load);
            else
                mem.run(pc, ry, control_unit.signals.WrMem, control_unit.signals.Byte, need_to_load);

            if (control_unit.signals.WrOut or control_unit.signals.Pila == bitset<2>(0b10))
                io.run(control_unit.signals.AddrIo, rx, control_unit.signals.WrOut, control_unit.signals.RdIn);

            if (control_unit.signals.Pila == bitset<2>(0b00))
                regfile.run(
                    control_unit.signals.A, 
                    control_unit.signals.B, 
                    control_unit.signals.D, 
                    alu.W, 
                    control_unit.signals.Wrd
                );
            else if (control_unit.signals.Pila == bitset<2>(0b01))
                regfile.run(
                    control_unit.signals.A, 
                    control_unit.signals.B, 
                    control_unit.signals.D, 
                    mem.rd_mem, 
                    control_unit.signals.Wrd
                );
            else if (control_unit.signals.Pila == bitset<2>(0b10)) {
                regfile.run(
                    control_unit.signals.A, 
                    control_unit.signals.B, 
                    control_unit.signals.D, 
                    io.rd_in, 
                    control_unit.signals.Wrd
                );
            }
            else
                regfile.run(
                    control_unit.signals.A, 
                    control_unit.signals.B, 
                    control_unit.signals.D, 
                    pc, 
                    control_unit.signals.Wrd
                );

            rx = regfile.A;
            ry = regfile.B;

            if (control_unit.signals.LdIr) ir = mem.rd_mem;

            if (control_unit.signals.LdPc) {
                if (control_unit.signals.AluR) pc = alu.W;
                else pc = r_at;
            }

            r_at = alu.W;
        }
        catch (InvalidInputError& e) {
            throw RuntimeError(ir, pc, state_to_string(control_unit.current_state), e);
        }
        catch (UnauthorizedDataPortAccessError& e) {
            throw RuntimeError(ir, pc, state_to_string(control_unit.current_state), e);
        }
        catch (UninitializedMemoryAccessError& e) {
            throw RuntimeError(ir, pc, state_to_string(control_unit.current_state), e);
        }
        catch (CapacityExceededError& e) {
            throw RuntimeError(ir, pc, state_to_string(control_unit.current_state), e);
        }
        catch (ConstantParsingError& e) {
            throw RuntimeError(ir, pc, state_to_string(control_unit.current_state), e);
        }
    }

    void execute() {
        while (true) cicle();
    }
};