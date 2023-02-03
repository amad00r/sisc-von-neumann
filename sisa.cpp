#include <sstream>
#include <iostream>
#include <filesystem>

#include "src/file.hpp"
#include "src/assembler/asm.hpp"
#include "src/virtual_machine/Processor.hpp"

#include "src/exceptions/argcError.hpp"
#include "src/exceptions/ExtensionError.hpp"
#include "src/exceptions/ActionError.hpp"

#include "src/exceptions/constant/CapacityExceededError.hpp"
#include "src/exceptions/constant/ConstantParsingError.hpp"

#include "src/exceptions/compile_error/AssemblerError.hpp"
#include "src/exceptions/compile_error/ParametersError.hpp"
#include "src/exceptions/compile_error/RegisterError.hpp"
#include "src/exceptions/compile_error/UndefinedInstructionError.hpp"

#include "src/exceptions/runtime_error/RuntimeError.hpp"
#include "src/exceptions/runtime_error/InvalidInputError.hpp"
#include "src/exceptions/runtime_error/UnauthorizedDataPortAccessError.hpp"
#include "src/exceptions/runtime_error/UninitializedMemoryAccessError.hpp"

using namespace std;

typedef vector< bitset<16> > MachineCode;

int main(int argc, char** argv) {
    try {
        if (argc != 3) throw argcError(argc);

        string action = argv[1];
        filesystem::path file_path = argv[2];

        if (action == "build") {
            if (file_path.extension() != ".sisa")
                throw ExtensionError("`build` expects a .sisa file", file_path.extension());

            MachineCode program(assemble_sisa(read_file(file_path)));

            file_path.replace_extension(".bin");
            write_binary(program, file_path);
        }
        else if (action == "run") {
            if (file_path.extension() == ".sisa") {
                Processor cpu;
                cpu.load(assemble_sisa(read_file(file_path)));
                cpu.mainloop();
            }
            else if (file_path.extension() == ".bin") {
                Processor cpu;
                cpu.load(read_binary(file_path));
                cpu.mainloop();
            }
            else throw ExtensionError("`run` expects a .sisa or .bin file", file_path.extension());
        }
        else if (action == "readbin") {
            if (file_path.extension() != ".bin")
                throw ExtensionError("`readbin` expects a .bin file", file_path.extension());
            print_binary_content(file_path, true);
        }
        else if (action == "readhex") {
            if (file_path.extension() != ".bin")
                throw ExtensionError("`readhex` expects a .bin file", file_path.extension());
            print_binary_content(file_path, false);
        }
        else throw ActionError(action);
    }
    catch (argcError& e) {
        e.explain();
    }
    catch (ExtensionError& e) {
        e.explain();
    }
    catch (ActionError& e) {
        e.explain();
    }
    catch (IOError& e) {
        e.explain();
    }
    catch (AssemblerError<CapacityExceededError>& e) {
        e.explain();
    }
    catch (AssemblerError<ConstantParsingError>& e) {
        e.explain();
    }
    catch (AssemblerError<ParametersError>& e) {
        e.explain();
    }
    catch (AssemblerError<RegisterError>& e) {
        e.explain();
    }
    catch (AssemblerError<UndefinedInstructionError>& e) {
        e.explain();
    }
    catch (RuntimeError<InvalidInputError>& e) {
        e.explain();
    }
    catch (RuntimeError<UnauthorizedDataPortAccessError>& e) {
        e.explain();
    }
    catch (RuntimeError<UninitializedMemoryAccessError>& e) {
        e.explain();
    }
    catch (RuntimeError<CapacityExceededError>& e) {
        e.explain();
    }
    catch (RuntimeError<ConstantParsingError>& e) {
        e.explain();
    }
    catch (char const* msg) {
        cerr << msg << endl;
    }
}