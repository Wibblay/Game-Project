#include "Camera.h"
#include "Config.h"
#include <iostream>

Camera::Camera() : cameraCoords(glm::vec2(0.0f, 0.0f)), zoomLevel(CameraConfig::DEFAULT_ZOOM_LEVEL), rotation(0) {}

Camera::~Camera() {}

void Camera::Move(glm::vec2 delta) 
{
    cameraCoords += delta;
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

    float translationAmount = 1.0f * zoomLevel;  
    switch (rotation) 
    {
        case 1:  // 90° clockwise
            Move(glm::vec2(translationAmount, -translationAmount));
            break;
        case 2:  // 180° (flipped)
            Move(glm::vec2(-translationAmount, -translationAmount));
            break;
        case 3:  // 270° clockwise
            Move(glm::vec2(-translationAmount, translationAmount));
            break;
        case 0:  // Back to 0° (reset)
            Move(glm::vec2(translationAmount, translationAmount));
            break;
    }  
}

void Camera::RotateCounterclockwise() 
{
    rotation = (rotation - 1 + 4) % 4;

    // Translate the camera to compensate for the rotation
    float translationAmount = 1.0f * zoomLevel;
    switch (rotation) 
    {
        case 3:  // 270° counterclockwise
            Move(glm::vec2(translationAmount, -translationAmount));
            break;
        case 2:  // 180° (flipped)
            Move(glm::vec2(-translationAmount, -translationAmount));
            break;
        case 1:  // 90° counterclockwise
            Move(glm::vec2(-translationAmount, translationAmount));
            break;
        case 0:  // Back to 0° (reset)
            Move(glm::vec2(translationAmount, translationAmount));
            break;
    }  
}

int Camera::GetRotation() const 
{
    return rotation;
}

glm::vec2 Camera::GetCoords() const
{
    return cameraCoords;
}