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

        // Handle events keys
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // input keyboard
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        cpu.port1 |= (1 << 4); // move left
                        break;
                    case SDLK_RIGHT:
                        cpu.port1 |= (1 << 5); // move right
                        break;
                    case SDLK_SPACE:
                        cpu.port1 |= (1 << 2); // fire
                        break;
                    case SDLK_RETURN:
                        cpu.port1 |= (1 << 1); // start 1 player
                        break;
                }
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        cpu.port1 &= ~(1 << 4); // stop move left
                        break;
                    case SDLK_RIGHT:
                        cpu.port1 &= ~(1 << 5); // stop move right
                        break;
                    case SDLK_SPACE:
                        cpu.port1 &= ~(1 << 2); // stop fire
                        break;
                    case SDLK_RETURN:
                        cpu.port1 &= ~(1 << 1); // stop start 1 player
                        break;
                }
            }
        }

        // Handle events
        graphics.HandleEvents(running);

        // Print CPU state
        cpu.PrintState();
        std::cin.get(); // Pause after each cycle for debugging
    }

    return 0;
}