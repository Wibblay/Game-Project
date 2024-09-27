#pragma once

#include <SDL2/SDL.h>
#include "Camera.h"
#include "Renderer.h"
#include "Map.h"
#include "InputManager.h"

class Game 
{
public:
    Game();
    ~Game();

    void Init();                                         // Initialize SDL, OpenGL, etc.
    void Run();                                          // Main game loop

    bool IsRunning();                                    // Check if the game is running

private:
    void HandleInput();                                  // Handle user input
    void Update();                                       // Update game state
    void Render();                                       // Render the game

    bool isRunning;

    SDL_Window* window;
    SDL_GLContext glContext;

    Renderer* renderer;
    Noise* noise;
    Map* map;
    Camera* camera;
    InputManager* inputManager;
};