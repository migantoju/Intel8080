#include "cpu.h"
#include <fstream>
#include <iostream>
#include <cstring>

CPU8080::CPU8080() {
    Reset();
}

void CPU8080::Reset() {
    A = B = C = D = E = H = L = 0;
    SP = 0x0000;
    PC = 0xFFFF;
    flags = 0;
    std::memset(memory, 0, sizeof(memory));
}

void CPU8080::LoadProgram(const char* filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    file.read((char*)memory, sizeof(memory));
    file.close();
}

void CPU8080::PrintState() {
    std::cout << "A: " << std::hex << (int)A << " "
              << "B: " << std::hex << (int)B << " "
              << "C: " << std::hex << (int)C << " "
              << "D: " << std::hex << (int)D << " "
              << "E: " << std::hex << (int)E << " "
              << "H: " << std::hex << (int)H << " "
              << "L: " << std::hex << (int)L << std::endl;
    std::cout << "SP: " << std::hex << SP << " "
              << "PC: " << std::hex << PC << std::endl;
}

void CPU8080::EmulateCycle() {
    uint8_t opcode = memory[PC]; // Fetch opcode from memory
    PC++; // Increment program counter

    switch(opcode) {
        case 0x00: // NOP
            break;
        case 0x01: // LXI B, D16
            C = memory[PC];
            B = memory[PC + 1];
            PC += 2;
            break;
        case 0x02: // STAX B
            memory[(B << 8) | C] = A;
            break;
        case 0x03: // INX B
            C++;
            if (C == 0) B++;
            break;
        case 0x04: // INR B
            B++;
            break;
        case 0x05: // DCR B
            B--;
            break;
        case 0x06: // MVI B, D8
            B = memory[PC];
            PC++;
            break;
        default:
            std::cerr << "Error: Unimplemented opcode " << std::hex << (int)opcode << std::endl;
            break;
    }
}
