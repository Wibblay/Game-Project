#include "Camera.h"
#include "Config.h"
#include <iostream>

Camera::Camera() : cameraCoords(glm::vec2(0.0f, 0.0f)), zoomLevel(CameraConfig::DEFAULT_ZOOM_LEVEL), rotation(0) {}

Camera::~Camera() {}

void Camera::Move(glm::vec2 delta) 
{
    switch (rotation) 
    {
        case 0:  // 0° (default)
            cameraCoords += delta;
            break;
        case 1:  // 90° clockwise
            cameraCoords.x -= delta.y;
            cameraCoords.y += delta.x;
            break;
        case 2:  // 180° (flipped)
            cameraCoords -= delta;
            break;
        case 3:  // 270° clockwise
            cameraCoords.x += delta.y;
            cameraCoords.y -= delta.x;
            break;
    }  
}

void Camera::SetPosition(glm::vec2 newPosition) 
{
    cameraCoords = newPosition;
}

void Camera::Zoom(float zoomFactor) 
{
    zoomLevel += zoomFactor;
    if (zoomLevel < CameraConfig::MIN_ZOOM_LEVEL) 
    {
        zoomLevel = CameraConfig::MIN_ZOOM_LEVEL;
    }
    if (zoomLevel > CameraConfig::MAX_ZOOM_LEVEL) 
    {
        zoomLevel = CameraConfig::MAX_ZOOM_LEVEL;
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

glm::vec2 Camera::GetCoords() const
{
    return cameraCoords;
}