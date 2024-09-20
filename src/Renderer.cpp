#include "Renderer.h"
#include <GL/glew.h>
#include "Config.h"
#include <iostream>
#include <vector>
#include <algorithm>

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::InitOpenGL() 
{
    // Set background clear color and enable depth test
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark green background
}

void Renderer::RenderMap(Map& map, const Camera& camera) 
{
    const std::vector<Tile>& renderBuffer = map.CollectTileRenderData(camera);
    float tileSize = camera.GetZoomLevel();
    int rotation = camera.GetRotation();
    
    for (auto& tile : renderBuffer)
    {
        RenderTile(tile, tileSize, rotation);
    }
}

void Renderer::RenderTile(const Tile& tile, const float& tileSize, const int& rotation)
{
    RenderTopFace(tile, tileSize);
    if (tile.sideFacesVisibleFlags[rotation]){
        // RenderLeftFace(tile, tileSize);
        // RenderRightFace(tile, tileSize);
        RenderSideFaces(tile, tileSize);
    }
    
}

void Renderer::RenderTopFace(const Tile& tile, const float& tileSize)
{
    // Render the top face (normal fill)
    glBegin(GL_QUADS);
    glColor3f(0.14f, 0.9f, 0.37f);  // Tile color

    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2) + tile.height);  // Top
    glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y + tile.height);  // Right
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y + (tileSize / 2) + tile.height);  // Bottom
    glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y + tile.height);  // Left

    glEnd();

    // // Render the outline (wireframe mode)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Set to wireframe mode
    // glLineWidth(2.0f);  // Set the width of the line (thickness)
    // glColor3f(0.2f, 0.2f, 0.2f);  // Dark grey for outline

    // glBegin(GL_QUADS);
    
    // glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2) + tile.height);  // Top
    // glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y + tile.height);  // Right
    // glVertex2f(tile.tileCoords.x, tile.tileCoords.y + (tileSize / 2) + tile.height);  // Bottom
    // glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y + tile.height);  // Left

    // glEnd();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Set back to fill mode
}

void Renderer::RenderSideFaces(const Tile& tile, const float& tileSize)
{
    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.82f, 0.23f);  // Tile color

    glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y + tile.height);  // Top Left
    glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y);                // Bottom Left
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2));                // Bottom Middle
    glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y);                // Bottom Right
    glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y + tile.height);  // Top Right
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2) + tile.height);  // Top Middle

    glEnd();
}

void Renderer::RenderLeftFace(const Tile& tile, const float& tileSize)
{
    // Render the left face (normal fill)
    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.82f, 0.23f);  // Tile color

    glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y + tile.height);  // Top
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2) + tile.height);  // Right
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2));                // Bottom
    glVertex2f(tile.tileCoords.x - (tileSize / 2), tile.tileCoords.y);                // Left

    glEnd();
}

void Renderer::RenderRightFace(const Tile& tile, const float& tileSize)
{
    // Render the right face (normal fill)
    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.82f, 0.23f);  // Tile color

    glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y + tile.height);  // Top
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2) + tile.height);  // Left
    glVertex2f(tile.tileCoords.x, tile.tileCoords.y - (tileSize / 2));                // Bottom
    glVertex2f(tile.tileCoords.x + (tileSize / 2), tile.tileCoords.y);                // Right

    glEnd();
}