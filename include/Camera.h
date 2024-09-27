#pragma once

#include <glm\glm.hpp>

namespace CameraConfig
{
    inline constexpr float MIN_ZOOM_LEVEL = 20.0f;       // Minimum zoom level
    inline constexpr float MAX_ZOOM_LEVEL = 50.0f;       // Maximum zoom level
    inline constexpr float DEFAULT_ZOOM_LEVEL = 35.0f;   // Default zoom level
    inline constexpr float ZOOM_INCREMENT_VALUE = 1.0f; //Size of zoom increment
}

class Camera 
{
public:
    Camera();
    ~Camera();

    // Camera position and movement
    void Move(glm::vec2 delta);
    void SetPosition(glm::vec2 newPosition);

    // Zoom controls
    void Zoom(float zoomFactor);
    float GetZoomLevel() const;

    // Rotation controls
    void RotateClockwise();
    void RotateCounterclockwise();
    int GetRotation() const;  // Get the current rotation (0, 1, 2, 3)

    // Getters for camera position
    glm::vec2 GetCoords() const;

private:
    glm::vec2 cameraCoords; 
    float zoomLevel;
    int rotation;       // 0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°
};
