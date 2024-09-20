#include "graphics.h"
#include <iostream>

Graphics::Graphics() : window(nullptr), renderer(nullptr), texture(nullptr) {
}


Graphics::~Graphics() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: Could not initialize SDL2: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Error: Could not create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error: Could not create renderer: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!texture) {
        std::cerr << "Error: Could not create texture: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void Graphics::Clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clean to black colors
    SDL_RenderClear(renderer);
}

void Graphics::DrawPixel(int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
    SDL_RenderDrawPoint(renderer, x, y);
}

void Graphics::Update() {
    SDL_RenderPresent(renderer); // Update the screen with the renderer content
}

void Graphics::HandleEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}