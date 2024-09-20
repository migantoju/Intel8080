#ifndef CPU_H
#define CPU_H

#include <cstdint>

class CPU8080 {
public:
    uint8_t A, B, C, D, E, H, L; // General purpose registers and accumulator
    uint16_t SP, PC; // Stack pointer and program counter
    uint8_t flags; // Flags register

    uint8_t memory[0x10000]; // 64KB of memory

    CPU8080();
    void Reset(); // Reset the CPU to its initial state
    void LoadProgram(const char* filename); // Load a program into memory
    void EmulateCycle(); // Emulate a single cycle
    void PrintState(); // Print the state of the CPU

};

#endif