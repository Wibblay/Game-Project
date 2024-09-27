#pragma once

#include <glm\glm.hpp>

class Camera 
{
public:
    Camera();
    ~Camera();

    // Camera position and movement
    void Move(glm::vec2 delta);
    void SetPosition(glm::vec2 newPosition);
    glm::vec2 GetCoords() const;

    // Zoom controls
    void Zoom(const float& zoomDirection);
    float GetZoomLevel() const;

    // Rotation controls
    void RotateClockwise();
    void RotateCounterclockwise();
    int GetRotation() const;  // Get the current rotation (0, 1, 2, 3)   

private:
    glm::vec2 cameraCoords; 
    float zoomLevel;
    int rotation;       // 0 = NE, 1 = NW, 2 = SW, 3 = SE

    // Zoom config
    const float minZoomLevel;
    const float maxZoomLevel;
    const float zoomIncrement;
};
