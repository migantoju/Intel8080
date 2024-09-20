#include "cpu.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    CPU8080 cpu;
    cpu.LoadProgram(argv[1]);

    while(true) {
        cpu.EmulateCycle();
        cpu.PrintState();
        std::cin.get(); // Pause after each cycle for debugging
    }

    return 0;
}