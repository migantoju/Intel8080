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
        case 0x07: // RLC
            flags = (A & 0x80) ? 1 : 0;
            A = (A << 1) | (A >> 7);
            break;
        case 0x08: // -
            break;
        case 0x09: // DAD B
            {
                uint16_t HL = (H << 8) | L;
                uint16_t BC = (B << 8) | C;
                uint32_t result = HL + BC;
                H = (result >> 8) & 0xFF;
                L = result & 0xFF;
                flags = (result > 0xFFFF) ? 1 : 0;
            }
            break;
        case 0x0A: // LDAX B
            A = memory[(B << 8) | C];
            break;
        case 0x0B: // DCX B
            C--;
            if (C == 0xFF) B--;
            break;
        case 0x0C: // INR C
            C++;
            break;
        case 0x0D: // DCR C
            C--;
            break;
        case 0x0E: // MVI C, D8
            C = memory[PC];
            PC++;
            break;
        case 0x0F: // RRC
            flags = (A & 0x01) ? 1 : 0;
            A = (A >> 1) | (A << 7);
            break;
        case 0x10: // -
            break;
        case 0x11: // LXI D, D16
            E = memory[PC];
            D = memory[PC + 1];
            PC += 2;
            break;
        case 0x12: // STAX D
            memory[(D << 8) | E] = A;
            break;
        case 0x13: // INX D
            E++;
            if (E == 0) D++;
            break;
        case 0x14: // INR D
            D++;
            break;
        case 0x15: // DCR D
            D--;
            break;
        case 0x16: // MVI D, D8
            D = memory[PC];
            PC++;
            break;
        case 0x17: // RAL
            {
                uint8_t carry = flags;
                flags = (A & 0x80) ? 1 : 0;
                A = (A << 1) | carry;
            }
            break;
        case 0x18: // -
            break;
        case 0x19: // DAD D
            {
                uint16_t HL = (H << 8) | L;
                uint16_t DE = (D << 8) | E;
                uint32_t result = HL + DE;
                H = (result >> 8) & 0xFF;
                L = result & 0xFF;
                flags = (result > 0xFFFF) ? 1 : 0;
            }
            break;
        case 0x1A: // LDAX D
            A = memory[(D << 8) | E];
            break;
        case 0x1B: // DCX D
            E--;
            if (E == 0xFF) D--;
            break;
        case 0x1C: // INR E
            E++;
            break;
        case 0x1D: // DCR E
            E--;
            break;
        case 0x1E: // MVI E, D8
            E = memory[PC];
            PC++;
            break;
        case 0x1F: // RAR
            {
                uint8_t carry = flags;
                flags = (A & 0x01) ? 1 : 0;
                A = (A >> 1) | (carry << 7);
            }
            break;
        case 0x20: // -
            break;
        case 0x21: // LXI H, D16
            L = memory[PC];
            H = memory[PC + 1];
            PC += 2;
            break;
        case 0x22: // SHLD adr
            {
                uint16_t adr = memory[PC] | (memory[PC + 1] << 8);
                memory[adr] = L;
                memory[adr + 1] = H;
                PC += 2;
            }
            break;
        case 0x23: // INX H
            L++;
            if (L == 0) H++;
            break;
        case 0x24: // INR H
            H++;
            break;
        case 0x25: // DCR H
            H--;
            break;
        case 0x26: // MVI H, D8
            H = memory[PC];
            PC++;
            break;
        case 0x27: // DAA
            {
                uint16_t result = A;
                if ((result & 0x0F) > 9 || (flags & 0x10)) result += 6;
                if ((result & 0xF0) > 0x90 || (flags & 0x10)) {
                    result += 0x60;
                    flags |= 0x10;
                }
                A = result & 0xFF;
                flags &= 0x01;
            }
            break;
        case 0x28: // -
            break;
        case 0x29: // DAD H
            {
                uint16_t HL = (H << 8) | L;
                uint32_t result = HL + HL;
                H = (result >> 8) & 0xFF;
                L = result & 0xFF;
                flags = (result > 0xFFFF) ? 1 : 0;
            }
            break;
        case 0x2A: // LHLD adr
            {
                uint16_t adr = memory[PC] | (memory[PC + 1] << 8);
                L = memory[adr];
                H = memory[adr + 1];
                PC += 2;
            }
            break;
        case 0x2B: // DCX H
            L--;
            if (L == 0xFF) H--;
            break;
        case 0x2C: // INR L
            L++;
            break;
        case 0x2D: // DCR L
            L--;
            break;
        case 0x2E: // MVI L, D8
            L = memory[PC];
            PC++;
            break;
        case 0x2F: // CMA
            A = ~A;
            break;
        case 0x30: // -
            break;
        case 0x31: // LXI SP, D16
            SP = memory[PC] | (memory[PC + 1] << 8);
            PC += 2;
            break;
        case 0x32: // STA adr
            {
                uint16_t adr = memory[PC] | (memory[PC + 1] << 8);
                memory[adr] = A;
                PC += 2;
            }
            break;
        case 0x33: // INX SP
            SP++;
            break;
        case 0x34: // INR M
            {
                uint16_t adr = (H << 8) | L;
                memory[adr]++;
            }
            break;
        case 0x35: // DCR M
            {
                uint16_t adr = (H << 8) | L;
                memory[adr]--;
            }
            break;
        case 0x36: // MVI M, D8
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = memory[PC];
                PC++;
            }
            break;
        case 0x37: // STC
            flags |= 0x01;
            break;
        case 0x38: // -
            break;
        case 0x39: // DAD SP
            {
                uint16_t HL = (H << 8) | L;
                uint32_t result = HL + SP;
                H = (result >> 8) & 0xFF;
                L = result & 0xFF;
                flags = (result > 0xFFFF) ? 1 : 0;
            }
            break;
        case 0x3A: // LDA adr
            {
                uint16_t adr = memory[PC] | (memory[PC + 1] << 8);
                A = memory[adr];
                PC += 2;
            }
            break;
        case 0x3B: // DCX SP
            SP--;
            break;
        case 0x3C: // INR A
            A++;
            break;
        case 0x3D: // DCR A
            A--;
            break;
        case 0x3E: // MVI A, D8
            A = memory[PC];
            PC++;
            break;
        case 0x3F: // CMC
            flags ^= 0x01;
            break;
        case 0x40: // MOV B, B
            B = B;
            break;
        case 0x41: // MOV B, C
            B = C;
            break;
        case 0x42: // MOV B, D
            B = D;
            break;
        case 0x43: // MOV B, E
            B = E;
            break;
        case 0x44: // MOV B, H
            B = H;
            break;
        case 0x45: // MOV B, L
            B = L;
            break;
        case 0x46: // MOV B, M
            {
                uint16_t adr = (H << 8) | L;
                B = memory[adr];
            }
            break;
        case 0x47: // MOV B, A
            B = A;
            break;
        case 0x48: // MOV C, B
            C = B;
            break;
        case 0x49: // MOV C, C
            C = C;
            break;
        case 0x4A: // MOV C, D
            C = D;
            break;
        case 0x4B: // MOV C, E
            C = E;
            break;
        case 0x4C: // MOV C, H
            C = H;
            break;
        case 0x4D: // MOV C, L
            C = L;
            break;
        case 0x4E: // MOV C, M
            {
                uint16_t adr = (H << 8) | L;
                C = memory[adr];
            }
            break;
        case 0x4F: // MOV C, A
            C = A;
            break;
        case 0x50: // MOV D, B
            D = B;
            break;
        case 0x51: // MOV D, C
            D = C;
            break;
        case 0x52: // MOV D, D
            D = D;
            break;
        case 0x53: // MOV D, E
            D = E;
            break;
        case 0x54: // MOV D, H
            D = H;
            break;
        case 0x55: // MOV D, L
            D = L;
            break;
        case 0x56: // MOV D, M
            {
                uint16_t adr = (H << 8) | L;
                D = memory[adr];
            }
            break;
        case 0x57: // MOV D, A
            D = A;
            break;
        case 0x58: // MOV E, B
            E = B;
            break;
        case 0x59: // MOV E, C
            E = C;
            break;
        case 0x5A: // MOV E, D
            E = D;
            break;
        case 0x5B: // MOV E, E
            E = E;
            break;
        case 0x5C: // MOV E, H
            E = H;
            break;
        case 0x5D: // MOV E, L
            E = L;
            break;
        case 0x5E: // MOV E, M
            {
                uint16_t adr = (H << 8) | L;
                E = memory[adr];
            }
            break;
        case 0x5F: // MOV E, A
            E = A;
            break;
        case 0x60: // MOV H, B
            H = B;
            break;
        case 0x61: // MOV H, C
            H = C;
            break;
        case 0x62: // MOV H, D
            H = D;
            break;
        case 0x63: // MOV H, E
            H = E;
            break;
        case 0x64: // MOV H, H
            H = H;
            break;
        case 0x65: // MOV H, L
            H = L;
            break;
        case 0x66: // MOV H, M
            {
                uint16_t adr = (H << 8) | L;
                H = memory[adr];
            }
            break;
        case 0x67: // MOV H, A
            H = A;
            break;
        case 0x68: // MOV L, B
            L = B;
            break;
        case 0x69: // MOV L, C
            L = C;
            break;
        case 0x6A: // MOV L, D
            L = D;
            break;
        case 0x6B: // MOV L, E
            L = E;
            break;
        case 0x6C: // MOV L, H
            L = H;
            break;
        case 0x6D: // MOV L, L
            L = L;
            break;
        case 0x6E: // MOV L, M
            {
                uint16_t adr = (H << 8) | L;
                L = memory[adr];
            }
            break;
        case 0x6F: // MOV L, A
            L = A;
            break;
        case 0x70: // MOV M, B
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = B;
            }
            break;
        case 0x71: // MOV M, C
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = C;
            }
            break;
        case 0x72: // MOV M, D
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = D;
            }
            break;
        case 0x73: // MOV M, E
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = E;
            }
            break;
        case 0x74: // MOV M, H
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = H;
            }
            break;
        case 0x75: // MOV M, L
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = L;
            }
            break;
        case 0x76: // HLT
            std::exit(0);
            break;
        case 0x77: // MOV M, A
            {
                uint16_t adr = (H << 8) | L;
                memory[adr] = A;
            }
            break;
        case 0x78: // MOV A, B
            A = B;
            break;
        case 0x79: // MOV A, C
            A = C;
            break;
        case 0x7A: // MOV A, D
            A = D;
            break;
        case 0x7B: // MOV A, E
            A = E;
            break;
        case 0x7C: // MOV A, H
            A = H;
            break;
        case 0x7D: // MOV A, L
            A = L;
            break;
        case 0x7E: // MOV A, M
            {
                uint16_t adr = (H << 8) | L;
                A = memory[adr];
            }
            break;
        case 0x7F: // MOV A, A
            A = A;
            break;
        case 0x80: // ADD B
            {
                uint16_t result = A + B;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x81: // ADD C
            {
                uint16_t result = A + C;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x82: // ADD D
            {
                uint16_t result = A + D;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x83: // ADD E
            {
                uint16_t result = A + E;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x84: // ADD H
            {
                uint16_t result = A + H;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x85: // ADD L
            {
                uint16_t result = A + L;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x86: // ADD M
            {
                uint16_t adr = (H << 8) | L;
                uint16_t result = A + memory[adr];
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x87: // ADD A
            {
                uint16_t result = A + A;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x88: // ADC B
            {
                uint16_t result = A + B + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x89: // ADC C
            {
                uint16_t result = A + C + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8A: // ADC D
            {
                uint16_t result = A + D + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8B: // ADC E
            {
                uint16_t result = A + E + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8C: // ADC H
            {
                uint16_t result = A + H + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8D: // ADC L
            {
                uint16_t result = A + L + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8E: // ADC M
            {
                uint16_t adr = (H << 8) | L;
                uint16_t result = A + memory[adr] + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x8F: // ADC A
            {
                uint16_t result = A + A + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x90: // SUB B
            {
                uint16_t result = A - B;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x91: // SUB C
            {
                uint16_t result = A - C;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x92: // SUB D
            {
                uint16_t result = A - D;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x93: // SUB E
            {
                uint16_t result = A - E;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x94: // SUB H
            {
                uint16_t result = A - H;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x95: // SUB L
            {
                uint16_t result = A - L;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x96: // SUB M
            {
                uint16_t adr = (H << 8) | L;
                uint16_t result = A - memory[adr];
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x97: // SUB A
            {
                uint16_t result = A - A;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x98: // SBB B
            {
                uint16_t result = A - B - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x99: // SBB C
            {
                uint16_t result = A - C - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9A: // SBB D
            {
                uint16_t result = A - D - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9B: // SBB E
            {
                uint16_t result = A - E - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9C: // SBB H
            {
                uint16_t result = A - H - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9D: // SBB L
            {
                uint16_t result = A - L - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9E: // SBB M
            {
                uint16_t adr = (H << 8) | L;
                uint16_t result = A - memory[adr] - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0x9F: // SBB A
            {
                uint16_t result = A - A - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xA0: // ANA B
            A &= B;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA1: // ANA C
            A &= C;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA2: // ANA D
            A &= D;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA3: // ANA E
            A &= E;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA4: // ANA H
            A &= H;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA5: // ANA L
            A &= L;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA6: // ANA M
            {
                uint16_t adr = (H << 8) | L;
                A &= memory[adr];
                flags = (A == 0) ? 1 : 0;
            }
            break;
        case 0xA7: // ANA A
            A &= A;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA8: // XRA B
            A ^= B;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xA9: // XRA C
            A ^= C;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xAA: // XRA D
            A ^= D;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xAB: // XRA E
            A ^= E;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xAC: // XRA H
            A ^= H;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xAD: // XRA L
            A ^= L;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xAE: // XRA M
            {
                uint16_t adr = (H << 8) | L;
                A ^= memory[adr];
                flags = (A == 0) ? 1 : 0;
            }
            break;
        case 0xAF: // XRA A
            A ^= A;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB0: // ORA B
            A |= B;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB1: // ORA C
            A |= C;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB2: // ORA D
            A |= D;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB3: // ORA E
            A |= E;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB4: // ORA H
            A |= H;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB5: // ORA L
            A |= L;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB6: // ORA M
            {
                uint16_t adr = (H << 8) | L;
                A |= memory[adr];
                flags = (A == 0) ? 1 : 0;
            }
            break;
        case 0xB7: // ORA A
            A |= A;
            flags = (A == 0) ? 1 : 0;
            break;
        case 0xB8: // CMP B
            {
                uint16_t result = A - B;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xB9: // CMP C
            {
                uint16_t result = A - C;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBA: // CMP D
            {
                uint16_t result = A - D;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBB: // CMP E
            {
                uint16_t result = A - E;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBC: // CMP H
            {
                uint16_t result = A - H;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBD: // CMP L
            {
                uint16_t result = A - L;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBE: // CMP M
            {
                uint16_t adr = (H << 8) | L;
                uint16_t result = A - memory[adr];
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xBF: // CMP A
            {
                uint16_t result = A - A;
                flags = (result > 0xFF) ? 1 : 0;
            }
            break;
        case 0xC0: // RNZ
            if (!(flags & 0x01)) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xC1: // POP B
            C = memory[SP];
            B = memory[SP + 1];
            SP += 2;
            break;
        case 0xC2: // JNZ adr
            if (!(flags & 0x01)) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xC3: // JMP adr
            PC = memory[PC] | (memory[PC + 1] << 8);
            break;
        case 0xC4: // CNZ adr
            if (!(flags & 0x01)) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xC5: // PUSH B
            memory[SP - 1] = B;
            memory[SP - 2] = C;
            SP -= 2;
            break;
        case 0xC6: // ADI D8
            {
                uint16_t result = A + memory[PC];
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
                PC++;
            }
            break;
        case 0xC7: // RST 0
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x00;
            break;
        case 0xC8: // RZ
            if (flags & 0x01) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xC9: // RET
            PC = memory[SP] | (memory[SP + 1] << 8);
            SP += 2;
            break;
        case 0xCA: // JZ adr
            if (flags & 0x01) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xCB: // -
            break;
        case 0xCC: // CZ adr
            if (flags & 0x01) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xCD: // CALL adr
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = memory[PC] | (memory[PC + 1] << 8);
            break;
        case 0xCE: // ACI D8
            {
                uint16_t result = A + memory[PC] + flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
                PC++;
            }
            break;
        case 0xCF: // RST 1
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x08;
            break;
        case 0xD0: // RNC
            if (!(flags & 0x10)) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xD1: // POP D
            E = memory[SP];
            D = memory[SP + 1];
            SP += 2;
            break;
        case 0xD2: // JNC adr
            if (!(flags & 0x10)) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xD3: // OUT D8
            std::cout << "OUT " << std::hex << (int)memory[PC] << std::endl;
            PC++;
            break;
        case 0xD4: // CNC adr
            if (!(flags & 0x10)) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xD5: // PUSH D
            memory[SP - 1] = D;
            memory[SP - 2] = E;
            SP -= 2;
            break;
        case 0xD6: // SUI D8
            {
                uint16_t result = A - memory[PC];
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
                PC++;
            }
            break;
        case 0xD7: // RST 2
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x10;
            break;
        case 0xD8: // RC
            if (flags & 0x10) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xD9: // -
            break;
        case 0xDA: // JC adr
            if (flags & 0x10) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xDB: // IN D8
            std::cout << "IN " << std::hex << (int)memory[PC] << std::endl;
            PC++;
            break;
        case 0xDC: // CC adr
            if (flags & 0x10) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xDD: // -
            break;
        case 0xDE: // SBI D8
            {
                uint16_t result = A - memory[PC] - flags;
                A = result & 0xFF;
                flags = (result > 0xFF) ? 1 : 0;
                PC++;
            }
            break;
        case 0xDF: // RST 3
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x18;
            break;
        case 0xE0: // RPO
            if (!(flags & 0x04)) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xE1: // POP H
            L = memory[SP];
            H = memory[SP + 1];
            SP += 2;
            break;
        case 0xE2: // JPO adr
            if (!(flags & 0x04)) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xE3: // XTHL
            {
                uint8_t temp = L;
                L = memory[SP];
                memory[SP] = temp;
                temp = H;
                H = memory[SP + 1];
                memory[SP + 1] = temp;
            }
            break;
        case 0xE4: // CPO adr
            if (!(flags & 0x04)) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xE5: // PUSH H
            memory[SP - 1] = H;
            memory[SP - 2] = L;
            SP -= 2;
            break;
        case 0xE6: // ANI D8
            A &= memory[PC];
            flags = (A == 0) ? 1 : 0;
            PC++;
            break;
        case 0xE7: // RST 4
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x20;
            break;
        case 0xE8: // RPE
            if (flags & 0x04) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xE9: // PCHL
            PC = (H << 8) | L;
            break;
        case 0xEA: // JPE adr
            if (flags & 0x04) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xEB: // XCHG
            {
                uint8_t temp = L;
                L = D;
                D = temp;
                temp = H;
                H = E;
                E = temp;
            }
            break;
        case 0xEC: // CPE adr
            if (flags & 0x04) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xED: // -
            break;
        case 0xEE: // XRI D8
            A ^= memory[PC];
            flags = (A == 0) ? 1 : 0;
            PC++;
            break;
        case 0xEF: // RST 5
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x28;
            break;
        case 0xF0: // RP
            if (!(flags & 0x08)) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xF1: // POP PSW
            A = memory[SP + 1];
            flags = memory[SP];
            SP += 2;
            break;
        case 0xF2: // JP adr
            if (!(flags & 0x08)) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xF3: // DI
            break;
        case 0xF4: // CP adr
            if (!(flags & 0x08)) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xF5: // PUSH PSW
            memory[SP - 1] = A;
            memory[SP - 2] = flags;
            SP -= 2;
            break;
        case 0xF6: // ORI D8
            A |= memory[PC];
            flags = (A == 0) ? 1 : 0;
            PC++;
            break;
        case 0xF7: // RST 6
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x30;
            break;
        case 0xF8: // RM
            if (flags & 0x08) {
                PC = memory[SP] | (memory[SP + 1] << 8);
                SP += 2;
            }
            break;
        case 0xF9: // SPHL
            SP = (H << 8) | L;
            break;
        case 0xFA: // JM adr
            if (flags & 0x08) {
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xFB: // EI
            break;
        case 0xFC: // CM adr
            if (flags & 0x08) {
                memory[SP - 1] = (PC >> 8) & 0xFF;
                memory[SP - 2] = PC & 0xFF;
                SP -= 2;
                PC = memory[PC] | (memory[PC + 1] << 8);
            } else {
                PC += 2;
            }
            break;
        case 0xFD: // -
            break;
        case 0xFE: // CPI D8
            {
                uint16_t result = A - memory[PC];
                flags = (result > 0xFF) ? 1 : 0;
                PC++;
            }
            break;
        case 0xFF: // RST 7
            memory[SP - 1] = (PC >> 8) & 0xFF;
            memory[SP - 2] = PC & 0xFF;
            SP -= 2;
            PC = 0x38;
            break;
        default:
            std::cerr << "Error: Unimplemented opcode " << std::hex << (int)opcode << std::endl;
            break;
    }
}
