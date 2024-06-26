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

    // reading 1 byte from memory
    Byte operator[](u32 address) const {
        return Data[address];
    }

    // writing 1 byte to memory
    Byte& operator[](u32 address) {
        return Data[address];
    }

    // writing 2 bytes to memory
    void WriteWord(u32& Cycles, Word Value, u32 Address) {
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -= 2;
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

    void Reset(Mem& memory) {
        PC = 0xFFFC;                    // set program counter to fffc, reset
        SP = 0x00FF;                    // set stack pointer to 00ff , reset
        D = 0;                          // clear decimal flag
        A = X = Y = 0;                  // set accumulator and registers to 0
        C = Z = I = D = B = V = N = 0;  // flags to 0
        memory.Initialise();
    }

    Byte FetchByte(u32 Cycles, Mem& memory) {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Byte ReadByte(u32 Cycles, Byte Address,Mem& memory) {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    Word FetchWord(u32 Cycles, Mem& memory) { 
        Word Data = memory[PC];
        PC++;
        Data = (memory[PC] << 8);
        PC++;
        Cycles -= 2;
        return Data;
    }


    static constexpr Byte INS_LDA_IM = 0xA9, 
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_JSR = 0x20;

    void LDASetStatus() {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void Execute(u32 Cycles, Mem& memory) {
        while (Cycles--) {
            Byte Ins = FetchByte(
                Cycles,
                memory);  // fetch instruction with help of program counter
            switch (Ins) {
                case INS_LDA_IM: {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDASetStatus();
                } break;
                case INS_LDA_ZP: {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                } break;
                case INS_LDA_ZPX: {
                    Byte ZeroPageAddr = FetchByte(Cycles, memory);
                    ZeroPageAddr += X;
                    Cycles--;
                    A = ReadByte(Cycles, ZeroPageAddr, memory);
                    LDASetStatus();
                } break;
                case INS_JSR: {
                    Word SubAddr = FetchWord(Cycles, memory);
                    memory.WriteWord(Cycles, PC - 1, SP);
                    PC = SubAddr;
                    SP++;
                    Cycles--;
                } break;
                default: {
                    printf("instruction not handled %d", Ins);
                } break;
            }
        }
    }
};

int main() {
    CPU cpu;
    Mem mem;
    cpu.Reset(mem);
    // start - inline program
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;  
    mem[0x4243] = 0x84;
    // end - inline program
    cpu.Execute(9, mem);

    return 0;
}