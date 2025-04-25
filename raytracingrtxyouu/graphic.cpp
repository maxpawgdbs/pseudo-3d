#include <cmath>
#include <chrono>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "map.h"
#include "player.h"
#include "config.h"

#define M_PI 3.14159265358979323846

int fps = 0;
void Update(Player Camera, GLFWwindow* window) {

	long long TimeThisFrame = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	if (TimeThisFrame - TimeCountFps < 1000) fps++;
	else {
		TimeCountFps = TimeThisFrame;
		std::cout << "fps: " << fps << '\n';
		fps = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Camera.MovementForward(TimeThisFrame, TimeLastFrame);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Camera.MovementBackward(TimeThisFrame, TimeLastFrame);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Camera.MovementRight(TimeThisFrame, TimeLastFrame);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Camera.MovementLeft(TimeThisFrame, TimeLastFrame);
	}

	glfwSetWindowUserPointer(window, &Camera);
	std::cout << Camera.Camera.x << ' ' << Camera.Camera.y << ' ' << Camera.RotationAngle << '\n';

	glfwSetCursorPosCallback(window,
		[](GLFWwindow* w, double xpos, double ypos) {
			auto Camera = static_cast<Player*>(glfwGetWindowUserPointer(w));
			Camera->mouse_callback(w, xpos, ypos);
		}
	);



	Camera.CheckCollision();

	for (int i = 0; i < ScreenWidth; i++) {
		double r = (Camera.RotationAngle + i * (120.0 / ScreenWidth)) / 180.0 * M_PI;
		double m = 1e10;
		double cosr = cos(r);
		double sinr = sin(r);
		double tanr = tan(r);
		/*Coords M1 = Camera.Camera;
		Coords M2;
		if (cosr < 0) M2 = { -1.0 * MapWidth + Camera.Camera.x, -MapWidth * tanr + Camera.Camera.y };
		else M2 = { 1.0 * MapWidth + Camera.Camera.x, MapWidth * tanr + Camera.Camera.y };

		if (cosr < 0 && sinr < 0) {
			M2 = { -1.0 * MapWidth + Camera.Camera.x, -MapWidth * tanr + Camera.Camera.y };
		}
		else if (cosr >= 0 && sinr >= 0) {
			M2 = { 1.0 * MapWidth + Camera.Camera.x, MapWidth * tanr + Camera.Camera.y };
		}
		else if (cosr < 0 && sinr >= 0) {
			M2 = { -1.0 * MapWidth + Camera.Camera.x, MapWidth * tanr + Camera.Camera.y };
		}
		else if (cosr >= 0 && sinr < 0) {
			M2 = { 1.0 * MapWidth + Camera.Camera.x, -MapWidth * tanr + Camera.Camera.y };
		}*/

		for (auto SegEl : MapSegments) {
			bool d1 = tanr * SegEl.a.x + Camera.Camera.y > SegEl.a.y;
			bool d2 = tanr * SegEl.b.x + Camera.Camera.y > SegEl.b.y;
			if (d1 != d2) {
				double fx;
				if (SegEl.a.x <= SegEl.b.x) {
					fx = (SegEl.b.y - SegEl.a.y) / (SegEl.b.x - SegEl.a.x);
				}
				else {
					fx = (SegEl.a.y - SegEl.b.y) / (SegEl.a.x - SegEl.b.x);
				}
				double fy = SegEl.a.y - SegEl.a.x * fx;
				double cy = Camera.Camera.y - Camera.Camera.x * tanr;
				double gx = (fy - cy) / (tanr - fx);
				if (gx * cosr < 0) continue;
				double gy = tanr * gx + Camera.Camera.y;
				m = std::min(m, sqrt(pow(Camera.Camera.x - gx, 2) + pow(Camera.Camera.y - gy, 2)));
				//m = 0.5;
			}
			/*Coords P1 = SegEl.a;
			Coords P2 = SegEl.b;
			double d1 = DotProduct(M1, M2, P1);
			double d2 = DotProduct(M1, M2, P2);
			double d3 = DotProduct(M1, P1, P2);
			double d4 = DotProduct(M2, P1, P2);
			if (d1 * d2 < 0 && d3 * d4 < 0) {
				double fx;
				if (SegEl.a.x <= SegEl.b.x) {
					fx = (SegEl.b.y - SegEl.a.y) / (SegEl.b.x - SegEl.a.x);
				}
				else {
					fx = (SegEl.a.y - SegEl.b.y) / (SegEl.a.x - SegEl.b.x);
				}
				double fy = SegEl.a.y - SegEl.a.x * fx;
				double cy = Camera.Camera.y - Camera.Camera.x * tanr;
				double gx = (fy - cy) / (tanr - fx);
				double gy = tanr * gx + Camera.Camera.y;
				m = std::min(m, sqrt(pow(gx, 2) + pow(gy, 2)));
				//m = std::min(m, sqrt(pow((fy - cy) / (tanr - fx), 2) + pow(tanr * ((fy - cy) / (tanr - fx)) + Camera.Camera.y, 2)));
			}*/
		}

		if (m == 1e10) continue;

		double dk = 1.0 / m * WallHeightCoefficient / 2.0;
		//dk = 0.1;
		glBegin(GL_LINES);
		glColor3f(dk * 0.5, dk * 0.5, dk * 0.5);//2.5
		glVertex2f((i - ScreenWidth / 2.0) / (ScreenWidth / 2.0), -dk);
		glVertex2f((i - ScreenWidth / 2.0) / (ScreenWidth / 2.0), dk);
		glEnd();
	}
	while (TimeControlFps - TimeLastFrame < 1000 / STABLE_FPS) TimeControlFps = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	TimeControlFps = TimeThisFrame;
	TimeLastFrame = TimeThisFrame;
}