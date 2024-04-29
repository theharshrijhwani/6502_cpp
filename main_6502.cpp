#include <iostream>
using namespace std;

struct CPU {
    using Byte = unsigned char;   // 8bit
    using Word = unsigned short;  // 16 bit
    Word PC;                      // program counter
    Word SP;                      // stack pointer
    Byte A, X, Y;                 // accumulator, registers
    Word PS;                      // processort status

    Byte C : 1;  // carry flag
    Byte Z : 1;  // zero flag
    Byte I : 1;  // interrupt disable
    Byte D : 1;  // decimal mode
    Byte B : 1;  // break flag
    Byte V : 1;  // overflow flag
    Byte N : 1;  // negative flag
};

int main() {
    CPU cpu;
    return 0;
}