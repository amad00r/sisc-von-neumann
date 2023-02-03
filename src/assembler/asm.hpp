#include <bitset>
#include <sstream>
#include <string>
#include <vector>

#include "../file.hpp"
#include "../bits.hpp"
#include "instruction/is_instruction.hpp"
#include "instruction/alu_function.hpp"
#include "instruction/args.hpp"
#include "instruction/clean_instruction.hpp"

#include "../exceptions/constant/CapacityExceededError.hpp"
#include "../exceptions/constant/ConstantParsingError.hpp"
#include "../exceptions/compile_error/AssemblerError.hpp"
#include "../exceptions/compile_error/ParametersError.hpp"
#include "../exceptions/compile_error/RegisterError.hpp"
#include "../exceptions/compile_error/UndefinedInstructionError.hpp"

using namespace std;

typedef vector< bitset<16> > MachineCode;

bitset<16> assemble_instruction(string& instruction) {
    smatch capture;

    if (is_al(instruction, capture)) {
        bitset<16> control_word = 0b0000000000000000;
        copy_bits(get_al_function(capture[1]), 0, control_word);
        copy_bits(get_register_id(capture[2]), 3, control_word);
        copy_bits(get_register_id(capture[3]), 9, control_word);
        if (capture[1] != "NOT")
            copy_bits(get_register_id(capture[4]), 6, control_word);
        return control_word;
    }
    if (is_cmp(instruction, capture)) {
        bitset<16> control_word = 0b0001000000000000;
        copy_bits(get_cmp_function(capture[1]), 0, control_word);
        copy_bits(get_register_id(capture[2]), 3, control_word);
        copy_bits(get_register_id(capture[3]), 9, control_word);
        copy_bits(get_register_id(capture[4]), 6, control_word);
        return control_word;
    }
    if (is_addi(instruction, capture)) {
        bitset<16> control_word = 0b0010000000000000;
        copy_bits(get_register_id(capture[1]), 6, control_word);
        copy_bits(get_register_id(capture[2]), 9, control_word);
        copy_bits(get_6bits_constant(capture[3]), 0, control_word);
        return control_word;
    }
    if (is_load(instruction, capture)) {
        bitset<16> control_word;
        if (capture[1] == "LD") control_word = 0b0011000000000000;
        else control_word = 0b0101000000000000;
        copy_bits(get_register_id(capture[2]), 6, control_word);
        copy_bits(get_6bits_constant(capture[3]), 0, control_word);
        copy_bits(get_register_id(capture[4]), 9, control_word);
        return control_word;
    }
    if (is_store(instruction, capture)) {
        bitset<16> control_word;
        if (capture[1] == "ST") control_word = 0b0100000000000000;
        else control_word = 0b0110000000000000;
        copy_bits(get_6bits_constant(capture[2]), 0, control_word);
        copy_bits(get_register_id(capture[3]), 9, control_word);
        copy_bits(get_register_id(capture[4]), 6, control_word);
        return control_word;
    }
    if (is_jalr(instruction, capture)) {
        bitset<16> control_word = 0b0111000000000000;
        copy_bits(get_register_id(capture[1]), 6, control_word);
        copy_bits(get_register_id(capture[2]), 9, control_word);
        return control_word;
    }
    if (is_bz(instruction, capture)) {
        bitset<16> control_word;
        if (capture[1] == "BZ") control_word = 0b1000000000000000;
        else control_word = 0b1000000100000000;
        copy_bits(get_register_id(capture[2]), 9, control_word);
        copy_bits(get_8bits_constant(capture[3]), 0, control_word);
        return control_word;
    }
    if (is_movi(instruction, capture)) {
        bitset<16> control_word;
        if (capture[1] == "MOVI") control_word = 0b1001000000000000;
        else control_word = 0b1001000100000000;
        copy_bits(get_register_id(capture[2]), 9, control_word);
        copy_bits(get_8bits_constant(capture[3]), 0, control_word);
        return control_word;
    }
    if (is_in(instruction, capture)) {
        bitset<16> control_word = 0b1010000000000000;
        copy_bits(get_register_id(capture[1]), 9, control_word);
        copy_bits(get_8bits_constant(capture[2]), 0, control_word);
        return control_word;
    }
    if (is_out(instruction, capture)) {
        bitset<16> control_word = 0b1010000100000000;
        copy_bits(get_8bits_constant(capture[1]), 0, control_word);
        copy_bits(get_register_id(capture[2]), 9, control_word);
        return control_word;
    }

    throw UndefinedInstructionError(instruction);
}

MachineCode assemble_sisa(stringstream sisa) {
    MachineCode mc;

    string instruction;
    int line_number = 1;
    while (getline(sisa, instruction)) {
        clean_instruction(instruction);
        if (not instruction.empty()) {
            try{
                mc.push_back(assemble_instruction(instruction));
            }
            catch (CapacityExceededError& e) {
                throw AssemblerError(line_number, instruction, e);
            }
            catch (ConstantParsingError& e) {
                throw AssemblerError(line_number, instruction, e);
            }
            catch (ParametersError& e) {
                throw AssemblerError(line_number, instruction, e);
            }
            catch (RegisterError& e) {
                throw AssemblerError(line_number, instruction, e);
            }
            catch (UndefinedInstructionError& e) {
                throw AssemblerError(line_number, instruction, e);
            }
        }
        ++line_number;
    }

    return mc;
}