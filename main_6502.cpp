#include <iostream>
using namespace std;

struct CPU {
    using Byte = unsigned char;   // 8bit
    using Word = unsigned short;  // 16 bit
    Word PC;                      // program counter
    Word SP;                      // stack pointer
    Byte A, X, Y;                 // accumulator, registers
    Word PS;                      // processor status

    Byte C : 1;  // carry flag
    Byte Z : 1;  // zero flag
    Byte I : 1;  // interrupt disable
    Byte D : 1;  // decimal mode
    Byte B : 1;  // break flag
    Byte V : 1;  // overflow flag
    Byte N : 1;  // negative flag

    void Reset() {
        PC = 0xFFFC;                    // set program counter to fffc, reset
        SP = 0x00FF;                    // set stack pointer to 00ff , reset
        D = 0;                          // clear decimal flag
        A = X = Y = 0;                  // set accumulator and registers to 0
        C = Z = I = D = B = V = N = 0;  // flags to 0
    }
};

int main() {
    CPU cpu;
    cpu.Reset();
    return 0;
}