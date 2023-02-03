#include <bitset>
#include <unordered_map>

#include "../../bits.hpp"

#include "../../exceptions/runtime_error/UninitializedMemoryAccessError.hpp"

using namespace std;

typedef vector< bitset<16> > MachineCode;

// 64Kb memory (65.536 bytes)
class Memory {
    private:
    unordered_map<bitset<16>, bitset<8>> mem;

    void st(const bitset<16>& addr, const bitset<16>& word) {
        if (addr[0]) {
            mem[bitset<16>(addr.to_ulong() - 1)] = lo_byte(word);
            mem[addr] = hi_byte(word);
        }
        else {
            mem[addr] = lo_byte(word);
            mem[bitset<16>(addr.to_ulong() + 1)] = hi_byte(word);
        }
    }

    void stb(const bitset<16>& addr, const bitset<16>& word) {
        mem[addr] = lo_byte(word);
    }

    bitset<16> ld(const bitset<16>& addr) {
        if (addr[0]) {
            bitset<16> lo_addr(addr.to_ulong() - 1);
            if (mem.find(addr) == mem.end() or mem.find(lo_addr) == mem.end())
                throw UninitializedMemoryAccessError("word", lo_addr);
            bitset<16> word;
            copy_bits(mem.at(lo_addr), 0, word);
            copy_bits(mem.at(addr), 8, word);
            return word;
        }
        else {
            bitset<16> hi_addr(addr.to_ulong() + 1);
            if (mem.find(addr) == mem.end() or mem.find(hi_addr) == mem.end())
                throw UninitializedMemoryAccessError("word", addr);
            bitset<16> word;
            copy_bits(mem.at(addr), 0, word);
            copy_bits(mem.at(hi_addr), 8, word);
            return word;
        }
    }

    bitset<16> ldb(const bitset<16>& addr) {
        if (mem.find(addr) == mem.end())
            throw UninitializedMemoryAccessError("byte", addr);
        return sign_extension(mem.at(addr));
    }

    public:
    bitset<16> rd_mem;

    void allocate_program(const MachineCode& program) {
        int size = program.size();
        for (int i = 0; i < size; ++i)
            st(bitset<16>(i*2), program[i]);
    }

    void run(const bitset<16>& addr_mem, const bitset<16>& wr_mem, bool WrMem, bool Byte, bool need_to_load) {
        if (Byte) {
            if (need_to_load) rd_mem = ldb(addr_mem);
            if (WrMem) stb(addr_mem, wr_mem);
        }
        else {
            if (need_to_load) rd_mem = ld(addr_mem);
            if (WrMem) st(addr_mem, wr_mem);
        }
    }
};
