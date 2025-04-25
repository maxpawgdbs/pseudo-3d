// player.cpp
#include "player.h"
#include <cmath>
#include "config.h"

#define M_PI 3.14159265358979323846

Player::Player()
    : Camera{ 0.0, 0.0 }
    , CameraBeforeMovement{ 0.0, 0.0 }
    , RotationAngle(0.0)
{}

void Player::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double sensitivity = 0.08;
    if (first_mouse) {
        last_mouse_x = xpos;
        first_mouse = false;
    }
    double xoffset = xpos - last_mouse_x;
    last_mouse_x = xpos;

    RotationAngle += xoffset * sensitivity;
    if (RotationAngle > 360.0) RotationAngle -= 360.0;
    if (RotationAngle < 0.0) RotationAngle += 360.0;
}

void Player::MovementForward(long long tNow, long long tLast) {
    double delta = (tNow - tLast) / 1000.0 * 3.0;
    double ang = (RotationAngle + 60.0) * M_PI / 180.0;
    Camera.x += delta * cos(ang);
    Camera.y += delta * sin(ang);
}

void Player::MovementBackward(long long tNow, long long tLast) {
    double delta = (tNow - tLast) / 1000.0 * 3.0;
    double ang = (RotationAngle + 60.0) * M_PI / 180.0;
    Camera.x -= delta * cos(ang);
    Camera.y -= delta * sin(ang);
}

void Player::MovementLeft(long long tNow, long long tLast) {
    double delta = (tNow - tLast) / 1000.0 * 3.0;
    double ang = (RotationAngle + 60.0 + 90.0) * M_PI / 180.0;
    Camera.x -= delta * cos(ang);
    Camera.y -= delta * sin(ang);
}

void Player::MovementRight(long long tNow, long long tLast) {
    double delta = (tNow - tLast) / 1000.0 * 3.0;
    double ang = (RotationAngle + 60.0 - 90.0) * M_PI / 180.0;
    Camera.x -= delta * cos(ang);
    Camera.y -= delta * sin(ang);
}

void Player::CheckCollision() {
    // Проверка по X
    if (CameraBeforeMovement.x != Camera.x) {
        for (const auto& el : MapSegments) {
            Coords M1 = CameraBeforeMovement;
            Coords M2 = Camera;
            Coords P1 = el.a, P2 = el.b;
            double d1 = DotProduct(M1, M2, P1);
            double d2 = DotProduct(M1, M2, P2);
            double d3 = DotProduct(M1, P1, P2);
            double d4 = DotProduct(M2, P1, P2);
            if (d1 * d2 < 0 && d3 * d4 < 0) {
                Camera.x = CameraBeforeMovement.x;
                break;
            }
        }
    }
    // Проверка по Y
    if (CameraBeforeMovement.y != Camera.y) {
        for (const auto& el : MapSegments) {
            Coords M1 = CameraBeforeMovement;
            Coords M2 = Camera;
            Coords P1 = el.a, P2 = el.b;
            double d1 = DotProduct(M1, M2, P1);
            double d2 = DotProduct(M1, M2, P2);
            double d3 = DotProduct(M1, P1, P2);
            double d4 = DotProduct(M2, P1, P2);
            if (d1 * d2 < 0 && d3 * d4 < 0) {
                Camera.y = CameraBeforeMovement.y;
                break;
            }
        }
    }
    CameraBeforeMovement = Camera;
}
