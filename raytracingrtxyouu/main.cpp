#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include "graphic.h"
#include "player.h"

void error_callback(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwSetErrorCallback(error_callback);

	int width = 1920;
	int height = 1080;
	GLFWwindow* window = glfwCreateWindow(width, height, "PSEVDO TRI D", NULL, NULL);
	glfwMaximizeWindow(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	Player Camera = Player();
	MapInit();
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Update(Camera, window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}