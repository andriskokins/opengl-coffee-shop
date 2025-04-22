#pragma once

#include <glm/glm.hpp>

#define BASE_SPEED 5.f
#define WALK_SPEED 2.5f
#define SPRINT_SPEED 15.f


struct SCamera
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed = 1.f;
    float MouseSensitivity = 0.1f; // Reduced for smoother mouse control
};

void InitCamera(SCamera& in)
{
    in.Front = glm::vec3(-0.85f, 0.0f, -0.5f);
    in.Position = glm::vec3(14.f, 2.5f, 10.0f); // Starting position
    in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
    in.WorldUp = in.Up;
    in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

    in.Yaw = -90.f;
    in.Pitch = 0.f;
}

// Process mouse movement to change camera orientation
void ProcessMouseMovement(SCamera& in, float xoffset, float yoffset)
{
    xoffset *= in.MouseSensitivity;
    yoffset *= in.MouseSensitivity;

    in.Yaw += xoffset;
    in.Pitch += yoffset;

    if (in.Pitch > 89.0f)
        in.Pitch = 89.0f;
    if (in.Pitch < -89.0f)
        in.Pitch = -89.0f;

    // Update Front, Right and Up Vectors
    glm::vec3 front;
    front.x = cos(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));
    front.y = sin(glm::radians(in.Pitch));
    front.z = sin(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));
    in.Front = glm::normalize(front);

    // Also recalculate the Right and Up vector
    in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
    in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}

// Process keyboard input to move the camera position
void ProcessKeyboard(SCamera& in, SCamera::Camera_Movement direction, double deltaTime)
{
    float velocity = in.MovementSpeed * (float)deltaTime;
    if (direction == SCamera::FORWARD)
        in.Position += in.Front * velocity;
    if (direction == SCamera::BACKWARD)
        in.Position -= in.Front * velocity;
    if (direction == SCamera::LEFT)
        in.Position -= in.Right * velocity;
    if (direction == SCamera::RIGHT)
        in.Position += in.Right * velocity;
    if (direction == SCamera::UP)
        in.Position += in.WorldUp * velocity;
    if (direction == SCamera::DOWN)
        in.Position -= in.WorldUp * velocity;

     //printf("Camera Pos\nx: %f y:%f z:%f\n", in.Position.x, in.Position.y, in.Position.z);
}
