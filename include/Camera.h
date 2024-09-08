#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>

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

#endif 
