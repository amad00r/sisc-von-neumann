# sisc-von-neumann

This project consists of two major parts:

- An assembler which translates plain SISA instructions into 16 bit
SISC Vonn Neumann instructions
- A virtual machine which can execute these 16 bit instructions by
emulating a SISC Von Neumann computer

Download the latest release for Linux or Windows from: https://github.com/amad00r/sisc-von-neumann/releases

## command-line usage

### build

To assemble sisa instructions into a file with machine code for the
SISC Von Neumann, use the command:

```
siscvn build /local/path/to/file.sisa
```

This command will create a .bin file with the same name and location as
the source file.

### run

To execute a program in the virtual machine, use the command:

```
siscvn run /local/path/to/file.bin
```

Or, if you do not want to mess with binary files and just execute a
sisa file, use the command: 

```
siscvn run /local/path/to/file.sisa
```

### readbin and readhex

To output in the terminal the binary or hexadecimal representation
of the binary instructions, use the command:

```
siscvn [readbin/readhex] /local/path/to/file.bin
```

## example of usage

Imagine this scenario: you are studying for an IC exam and you are asked
to write a program in SISA and translate it to hexadecimal instructions.
It must take a whole number as input and write its absolute value as output.

When you have finished thinking about the implementation of the
problem, writing down in paper the instructions, and doing its translation
into hexadecimal, it is the perfect time for using this tool.

### 1. writing the program

First, write a .sisa file with the solution you propose (in this case
you can find the solution in /examples/absolute_value.sisa):

```nasm
; we want to write to the printer the absolute value of the
; input that we get from the keyboard interpreted as Ca2

IN R0, 1
BZ R0, -2           ; wait for the keyboard req
IN R0, 0


MOVI R5, 0x00
MOVHI R5, 0x80      ; R5 contains a mask to check the highest bit
AND R7, R0, R5
BZ R7, 2            ; if the highest bit is 0, the number is positive
                    ; and we want to output it as it is
NOT R0, R0          ; else, we want to flip its bits and increment it in 1
ADDI R0, R0, 1


IN R7, 2
BZ R7, -2           ; wait for the printer req
OUT 0, R0
```

### 2. testing the program

Run this command to execute the program and test if it works as expected:

```
siscvn run examples/absolute_value.sisa
```

Keep tweaking your solution until the assembler compiles and the
program works as expected!

The solution mentioned above should execute like this:

```
$ siscvn run examples/absolute_value.sisa
Key-Req = 
```

First, it asks us to input a value (0 or 1) to indicate if the
keyboard data port is ready, or not. If we input 0, it will keep
asking us for the value of Key-Req indefinitely.

```
$ siscvn run examples/absolute_value.sisa
Key-Req = 1
KEY-DATA = 
```

After inputing 1, it asks us for a 16 bit value for the KEY-DATA
port, that we can input as a whole number or an hexadecimal.

```
$ siscvn run examples/absolute_value.sisa
Key-Req = 1
KEY-DATA = -123
0000000001111011
RuntimeError: execution of the program was aborted while state = FETCH, IR = 1010000100000000, PC = 0000000000011000
UninitializedMemoryAccessError: access to an uninitialized word stored in (0000000000011000) was attempted
```

After inputing -123, an arbitrary number I selected, we should see the
line: 0000000001111011, which, in fact, represents the number 123 in
decimal.

Finally, we see an error that can be ignored, which is intentional
and essential for the abortion of the execution. You can read more
about it below, in the Memory subsection of the SISC Von Neumann
virtual machine section.

### 3. checking the translation between sisa and machine code

Run these commands to create a binary file from the source code, and
output its contents in hexadecimal 16 bit instructions:

```
siscvn build examples/absolute_value.sisa
siscvn readhex examples/absolute_value.bin
```

If the assembler did not find any issues, it should have generated
this file: `examples/absolute_value.bin`, and then it should have outputted its content through the terminal like this:

```
$ siscvn build examples/absolute_value.sisa
$ siscvn readhex examples/absolute_value.bin
A001
80FE
A000
9A00
9B80
0178
8E02
0003
2001
AE02
8EFE
A100
```

Now, you can check if your translation from sisa to machine code was
correct.

## SISA assembler

The SISA assembler is currently under development.

it supports:

- comments with the semicolon syntax
- decimal and hexadecimal constants as instruction arguments
- all defined SISA instructions

it does not support:

- labels
- sections
- .data directives

## SISC Von Neumann virtual machine

The virtual machine consists of some components: Alu, InputOutput,
Memory, Regfile and ControlUnit. And some registers: RX, RY, IR, R@
and PC.

In this section I will discuss my approach to the implementation of some of these components in c++. This will bring light to the working of the virtual machine and explain why certain decisions were taken in the implementation. 

### Regfile

To simulate how the Regfile works, I used a `bitset<16>[8]` array. This way it is possible to access/store a value of any register by indexing its id.

It is important to mention that every register is being initialized
to 0. Since RX and RY registers are taking values from the Regfile
every cycle of the execution, registers must not be uninitialized in order to prevent unexpected behaviors.

### Memory

To simulate how the Memory works, I used an
`unordered_map<bitset<16>, bitset<8>>` data structure.

The program does not reserve 64kb of memory that are not likely to be used. Instead, it starts with an empty unordered_map that will grow in size when the program is allocated into memory (starting in 0x0000), and during its execution if some value is allocated into memory with LD or LDB.

Following this approach it is possible to throw a RuntimeError when
uninitialized memory is being accessed.

#### Program interruption

The important side-effect of having the ability to throw an
exception when an uninitialized memory access happens, is that the
virtual machine is now able to interrupt the execution of the
program when no more instructions are found.

Since the SISC Von Neumann does not have neither OS nor interruptions, it
does not have a mechanism to stop execution. The most elegant way I
found for dealing with this problem (while preserving SISC Von
Neumann features, such as being able to write code that dynamically
writes more code, or not having instructions and data segregation)
is by throwing an exception when it tries to fetch an instruction
from an uninitialized position in memory.

To determine that an UninitializedMemoryAccessError has happened because of
reaching the end of the program, take a look at the state of the virtual machine:
it should be FETCH.

### InputOutput

- KEY-DATA: port 0
- KEY-STATUS: port 1
- PRINT-DATA: port 0
- PRINT-STATUS: port 2

To simulate how the InputOutput works, I used the stdin for
the keyboard and the stdout for the printer.

When accessing keyboard data port using IN, the virtual machine will ask
the user to provide a 16 bit representable whole number or hexadecimal.

When writing to the printer data port using OUT, the virtual machine will output 16 bits and a newline through the stdout.

Remember that before accessing or writing to a data port, it is
necessary to check for its status port to see if the device is operative. If you skip this step, a RuntimeError will be thrown.

When accessing the keyboard status port, the virtual machine will ask
the user to provide 1 bit to simulate the Key-Req.

When accessing the printer, the Print-Req will automatically value 1.
As a side effect of writing to printer data port, Print-Req will value
0.
