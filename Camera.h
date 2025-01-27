#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <SDL.h>

enum CameraMode {
    FLIGHT,
    FPS
};

class Camera {
public:
    inline Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, CameraMode mode)
        : position(position), worldUp(up), yaw(yaw), pitch(pitch), mode(mode),
          movementSpeed(5.0f), mouseSensitivity(0.1f) {
        updateCameraVectors();
    }

    inline glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    inline void processKeyboard(SDL_Scancode key, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (key == SDL_SCANCODE_W) position += front * velocity;
        if (key == SDL_SCANCODE_S) position -= front * velocity;
        if (key == SDL_SCANCODE_A) position -= right * velocity;
        if (key == SDL_SCANCODE_D) position += right * velocity;

        if (mode == FLIGHT) {
            if (key == SDL_SCANCODE_Q) position -= up * velocity;
            if (key == SDL_SCANCODE_E) position += up * velocity;
        }
    }

    inline void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (mode == FPS) pitch = glm::clamp(pitch, -89.0f, 89.0f);
        }

        updateCameraVectors();
    }

    inline void switchMode(CameraMode newMode) {
        mode = newMode;
        if (mode == FPS) pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }

private:
    inline void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    CameraMode mode;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
};

#endif