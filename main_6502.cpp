#include <iostream>
using namespace std;

using Byte = unsigned char;   // 8 bit
using Word = unsigned short;  // 16 bit
using u32 = unsigned int;     // 4 bit

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise() {
        for (u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    // reading byte from memory
    Byte operator[](u32 address) const {
        return Data[address];
    }
};

struct CPU {
    Word PC;       // program counter
    Word SP;       // stack pointer
    Byte A, X, Y;  // accumulator, registers
    Word PS;       // processor status

    Byte C : 1;  // carry flag
    Byte Z : 1;  // zero flag
    Byte I : 1;  // interrupt disable
    Byte D : 1;  // decimal mode
    Byte B : 1;  // break flag
    Byte V : 1;  // overflow flag
    Byte N : 1;  // negative flag

    void Reset(Mem &memory) {
        PC = 0xFFFC;                    // set program counter to fffc, reset
        SP = 0x00FF;                    // set stack pointer to 00ff , reset
        D = 0;                          // clear decimal flag
        A = X = Y = 0;                  // set accumulator and registers to 0
        C = Z = I = D = B = V = N = 0;  // flags to 0
        memory.Initialise();
    }

    Byte FetchByte(u32 Cycles, Mem &memory) {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    void Execute(u32 Cycles, Mem &memory) {
        while (Cycles--) {
            Byte Ins = FetchByte(
                Cycles,
                memory);  // fetch instruction with help of program counter
            (void)Ins;
        }
    }
};

int main() {
    CPU cpu;
    Mem mem;
    cpu.Reset(mem);
    cpu.Execute(2, mem);
    return 0;
}