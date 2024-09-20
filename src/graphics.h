#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL.h"

class Graphics {

public:
    Graphics();
    ~Graphics();

    void Initialize(); // Initialize the graphics SDL2
    void Clear(); // Clear the screen
    void DrawPixel(int x, int y); // Draw a pixel on the screen
    void Update(); // Update the screen
    void HandleEvents(bool& running); // Handle SDL2 events

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    static const int SCREEN_WIDTH = 224; // Screen width
    static const int SCREEN_HEIGHT = 256; // Screen height

};

#endif