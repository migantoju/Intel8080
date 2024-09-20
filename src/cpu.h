#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "graphics.h"

class CPU8080 {
public:
    uint8_t A, B, C, D, E, H, L; // General purpose registers and accumulator
    uint16_t SP, PC; // Stack pointer and program counter
    uint8_t flags; // Flags register

    uint8_t memory[0x10000]; // 64KB of memory

    uint8_t InPort(uint8_t port); // Read from an input port

    uint8_t port1; // Buttons state for player 1
    uint8_t port2; // Buttons state for player 2 and others

    void OutPort(uint8_t port, uint8_t value); // Write to an output port

    CPU8080();
    void Reset(); // Reset the CPU to its initial state
    void LoadProgram(const char* filename); // Load a program into memory
    void EmulateCycle(); // Emulate a single cycle
    void PrintState(); // Print the state of the CPU

    void RenderGraphics(Graphics& graphics); // Render the graphics

private:
    uint8_t shiftRegister; // Register shift for graphics
    uint8_t shiftOffset; // Offset for shift registers graphics
};

#endif