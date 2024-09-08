#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include <unordered_map>

class InputManager 
{
public:
    InputManager();
    ~InputManager();

    void ProcessInput();                    // Process all input events (SDL Events)
    bool IsKeyPressed(SDL_Keycode key);     // Check if a specific key is pressed
    bool IsKeyJustPressed(SDL_Keycode key); // Check if a specific key was just pressed

    float GetScrollDelta();              // Get the current scroll delta

private:
    std::unordered_map<SDL_Keycode, bool> keyStates;         // Map to store the state of keys
    std::unordered_map<SDL_Keycode, bool> previousKeyStates; // Track previous key states to detect just pressed keys
    float scrollDelta;
};

#endif
