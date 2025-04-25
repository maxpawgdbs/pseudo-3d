#ifndef PLAYER_H
#define PLAYER_H

#include <glew.h>
#include <glfw3.h>
#include <cmath>
#include "map.h"

#define M_PI 3.14159265358979323846

class Player {
private:
    double last_mouse_x = 0.0;
    bool first_mouse = true;
    Coords CameraBeforeMovement;

public:
    Coords Camera;
    double RotationAngle{ 0.0 };
    Player();

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void MovementForward(long long TimeThisFrame, long long TimeLastFrame);
    void MovementBackward(long long TimeThisFrame, long long TimeLastFrame);
    void MovementLeft(long long TimeThisFrame, long long TimeLastFrame);
    void MovementRight(long long TimeThisFrame, long long TimeLastFrame);

    void CheckCollision();
};

#endif