#include "Camera.h"

#include "Config.h"
#include <iostream>

Camera::Camera() : cameraCoords(glm::vec2(0.0f, 0.0f)), zoomLevel(35.0f), rotation(0),
                    minZoomLevel(20.0f), maxZoomLevel(50.0f), zoomIncrement(1.0f)
{}

Camera::~Camera() {}

void Camera::Move(glm::vec2 delta) 
{
    switch (rotation) 
    {
        case 0:  // NE (default)
            cameraCoords += delta;
            break;
        case 1:  // NW 
            cameraCoords.x -= delta.y;
            cameraCoords.y += delta.x;
            break;
        case 2:  // SW (flipped)
            cameraCoords -= delta;
            break;
        case 3:  // SE
            cameraCoords.x += delta.y;
            cameraCoords.y -= delta.x;
            break;
    }  
}

void Camera::SetPosition(glm::vec2 newPosition) 
{
    cameraCoords = newPosition;
}

glm::vec2 Camera::GetCoords() const
{
    return cameraCoords;
}

void Camera::Zoom(const float& zoomDirection) 
{
    zoomLevel += zoomDirection * zoomIncrement;
    if (zoomLevel < minZoomLevel) 
    {
        zoomLevel = minZoomLevel;
    }
    if (zoomLevel > maxZoomLevel) 
    {
        zoomLevel = maxZoomLevel;
    }
}

float Camera::GetZoomLevel() const 
{
    return zoomLevel;
}

void Camera::RotateClockwise() 
{
    rotation = (rotation + 1) % 4;
}

void Camera::RotateCounterclockwise() 
{
    rotation = (rotation - 1 + 4) % 4;
}

int Camera::GetRotation() const 
{
    return rotation;
}

