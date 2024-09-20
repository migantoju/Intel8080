#include "cpu.h"
#include <iostream>
#include "graphics.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    CPU8080 cpu;
    Graphics graphics;

    graphics.Initialize();
    cpu.LoadProgram(argv[1]);

    bool running = true;

    while(running) {
        cpu.EmulateCycle();

        // Render graphics
        cpu.RenderGraphics(graphics);

        // Handle events
        graphics.HandleEvents(running);

        // Print CPU state
        cpu.PrintState();
        std::cin.get(); // Pause after each cycle for debugging
    }

    return 0;
}