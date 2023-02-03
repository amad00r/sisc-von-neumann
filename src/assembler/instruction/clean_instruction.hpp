#include <string>

void clean_instruction(string& instr) {
    size_t pos = instr.find_first_of(';');
    if (pos != string::npos) instr.erase(pos);
    instr.erase(0, instr.find_first_not_of(" \t"));
    instr.erase(instr.find_last_not_of(" \t") + 1);
}
