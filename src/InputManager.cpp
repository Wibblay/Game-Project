#include "InputManager.h"
#include "Config.h"

InputManager::InputManager() : scrollDelta(0.0f) {}

InputManager::~InputManager() {}

void InputManager::ProcessInput() 
{
    scrollDelta = 0.0f;
    previousKeyStates = keyStates;
    
    SDL_Event event;
    // Poll for all events (key presses, mouse events, etc.)
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            // Handle quit event, this could signal to the Game class to exit
            keyStates[SDLK_ESCAPE] = true; // We'll treat ESC as a quit signal for now
        }
        
        // Handle key down events
        if (event.type == SDL_KEYDOWN) 
        {
            SDL_Keycode key = event.key.keysym.sym;
            keyStates[key] = true;   // Mark the key as pressed
        }
        
        // Handle key up events
        if (event.type == SDL_KEYUP) 
        {
            SDL_Keycode key = event.key.keysym.sym;
            keyStates[key] = false;  // Mark the key as released
        }

        // Handle mouse wheel scrolling
        if (event.type == SDL_MOUSEWHEEL) 
        {
            if (event.wheel.y > 0) // Scroll up
            { 
                scrollDelta = CameraConfig::ZOOM_INCREMENT_VALUE;
            }
            else if (event.wheel.y < 0) // Scroll down
            { 
                scrollDelta = -CameraConfig::ZOOM_INCREMENT_VALUE;
            }
        }
    }
}

bool InputManager::IsKeyPressed(SDL_Keycode key) 
{
    // Return whether the key is currently pressed
    if (keyStates.find(key) != keyStates.end()) 
    {
        return keyStates[key];
    }
    return false;
}

// Check if the key was just pressed (pressed down but not held)
bool InputManager::IsKeyJustPressed(SDL_Keycode key) 
{
    if (keyStates[key] && !previousKeyStates[key]) 
    {
        return true;
    }
    return false;
}

float InputManager::GetScrollDelta() 
{
    return scrollDelta;
}