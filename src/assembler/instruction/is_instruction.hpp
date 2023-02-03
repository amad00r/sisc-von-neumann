#include <regex>
#include <string>

#include "../../exceptions/compile_error/ParametersError.hpp"

using namespace std;

bool is_al(const string& instr, smatch& capture) {
    regex r(R"((AND|OR|XOR|NOT|ADD|SUB|SHA|SHL)(\s.*|$))");
    if (not regex_match(instr, capture, r)) return false;

    if (capture[1] == "NOT") r = R"((NOT)\s+(\S+)\s*,\s*(\S+)\s*)";
    else r = R"((\S+)\s+(\S+)\s*,\s*(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r)) 
        throw ParametersError();

    return true;
}

bool is_cmp(const string& instr, smatch& capture) {
    regex r(R"((CMPLT|CMPLE|CMPEQ|CMPLTU|CMPLEU)(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"((\S+)\s+(\S+)\s*,\s*(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_addi(const string& instr, smatch& capture) {
    regex r(R"(ADDI(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"(ADDI\s+(\S+)\s*,\s*(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_load(const string& instr, smatch& capture) {
    regex r(R"(LDB?(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"((LDB?)\s+(\S+)\s*,\s*(\S+)\s*\(\s*(\S+)\s*\)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_store(const string& instr, smatch& capture) {
    regex r(R"(STB?(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"((STB?)\s+(\S+)\s*\(\s*(\S+)\s*\)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_jalr(const string& instr, smatch& capture) {
    regex r(R"(JALR(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"(JALR\s+(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_bz(const string& instr, smatch& capture) {
    regex r(R"(BN?Z(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"((BN?Z)\s+(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();

    return true;
}

bool is_movi(const string& instr, smatch& capture) {
    regex r(R"(MOVH?I(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"((MOVH?I)\s+(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();
    
    return true;
}

bool is_in(const string& instr, smatch& capture) {
    regex r(R"(IN(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"(IN\s+(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();
    
    return true;
}

bool is_out(const string& instr, smatch& capture) {
    regex r(R"(OUT(\s.*|$))");
    if (not regex_match(instr, r)) return false;

    r = R"(OUT\s+(\S+)\s*,\s*(\S+)\s*)";
    if (not regex_match(instr, capture, r))
        throw ParametersError();
    
    return true;
}