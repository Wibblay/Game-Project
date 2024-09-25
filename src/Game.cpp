#include "Game.h"
#include <iostream>
#include <GL/glew.h>
//#include <thread>
#include "Config.h"

Game::Game() : isRunning(false), window(nullptr), glContext(nullptr),
    renderer(nullptr), map(nullptr), camera(nullptr), inputManager(nullptr),
    windowWidth(WindowConfig::DEFAULT_WINDOW_HEIGHT), windowHeight(WindowConfig::DEFAULT_WINDOW_HEIGHT), 
    title(WindowConfig::WINDOW_TITLE) {}

Game::~Game() 
{
    // Cleanup
    delete renderer;
    delete noise;
    delete map;
    delete camera;
    delete inputManager;

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Init() 
{
    windowWidth = WindowConfig::DEFAULT_WINDOW_WIDTH;
    windowHeight = WindowConfig::DEFAULT_WINDOW_HEIGHT;
    title = WindowConfig::WINDOW_TITLE;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    // Create SDL Window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    if (!window) 
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) 
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Initialize GLEW after creating OpenGL context
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Set OpenGL attributes (clear color, depth test, etc.)
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);

    // Initialize game components
    renderer = new Renderer();
    renderer->InitOpenGL();

    camera = new Camera();        // Initialize the camera
    noise = new Noise(MapConfig::MAP_NOISE_SEED);
    map = new Map(*noise);              // Initialize the procedural map
    map->UpdateMapRenderBuffers(*camera, true);
    inputManager = new InputManager();  // Initialize input manager

    isRunning = true;
}

void Game::Run() 
{
    std::cout << "Running game" << std::endl;
    while (isRunning) 
    {
        HandleInput();
        Update();
        Render();
    }
}

void Game::HandleInput() 
{
    inputManager->ProcessInput();   // Handle SDL input events

    // Check for quit event
    if (inputManager->IsKeyPressed(SDLK_ESCAPE)) 
    {
        isRunning = false;
    }

    // Camera movement controls
    const float cameraSpeed = 2.0f / camera->GetZoomLevel();
    if (inputManager->IsKeyPressed(SDLK_UP)) 
    {
        camera->Move(glm::vec2(2 * cameraSpeed, 2 * cameraSpeed));  // Move camera up
    }
    if (inputManager->IsKeyPressed(SDLK_DOWN)) 
    {
        camera->Move(glm::vec2(-2 * cameraSpeed, -2 * cameraSpeed));   // Move camera down
    }
    if (inputManager->IsKeyPressed(SDLK_LEFT)) 
    {
        camera->Move(glm::vec2(-cameraSpeed, cameraSpeed));  // Move camera left
    }
    if (inputManager->IsKeyPressed(SDLK_RIGHT)) 
    {
        camera->Move(glm::vec2(cameraSpeed, -cameraSpeed));   // Move camera right
    }

    // Rotate the camera
    if (inputManager->IsKeyJustPressed(SDLK_PERIOD)) 
    {  // . key to rotate clockwise
        camera->RotateClockwise();
    }
    if (inputManager->IsKeyJustPressed(SDLK_COMMA)) 
    {   // , key to rotate counterclockwise
        camera->RotateCounterclockwise();
    }

    if (inputManager->IsKeyJustPressed(SDLK_0))
    {
        map->UpdateMapRenderBuffers(*camera, true);
    }

    if (inputManager->IsKeyJustPressed(SDLK_9))
    {
        std::cout << "Camera coords: " << camera->GetCoords().x << camera->GetCoords().y << std::endl;
    }

    // Handle zoom input from the mouse scroll wheel
    float scrollDelta = inputManager->GetScrollDelta();
    if (scrollDelta != 0.0f) 
    {
        camera->Zoom(scrollDelta * 0.1f);  // Scale the zoom change to a smaller amount
    }
}

void Game::Update() 
{
    // map->UpdateMapRenderBuffers(*camera, false);
}

void Game::Render() 
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the game world (map, player, etc.)
    renderer->RenderMap(*map, *camera);

    // Swap buffers to display the frame
    SDL_GL_SwapWindow(window);
}

bool Game::IsRunning() 
{
    return isRunning;
}