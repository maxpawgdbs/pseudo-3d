#ifndef UPDATE_H
#define UPDATE_H

#include <glew.h>
#include <glfw3.h>
#include <cmath>
#include <chrono>
#include <iostream>
#include "map.h"
#include "player.h"
#include "config.h"

#define M_PI 3.14159265358979323846

extern int fps;

void Update(Player Camera, GLFWwindow* window);

#endif
