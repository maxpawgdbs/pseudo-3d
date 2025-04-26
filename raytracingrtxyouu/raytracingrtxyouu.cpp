#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

# define M_PI           3.14159265358979323846

struct Coords {
	double x;
	double y;
};
struct Segment {
	Coords a;
	Coords b;
	double fkx, fb;
};
void error_callback(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
}

double rotate = 0.0;
double last_mouse_x = 0.0;
bool first_mouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static double sensitivity = 0.08;

	if (first_mouse) {
		last_mouse_x = xpos;
		first_mouse = false;
	}

	double xoffset = xpos - last_mouse_x;
	last_mouse_x = xpos;

	rotate += xoffset * sensitivity;

	if (rotate > 360.0) rotate -= 360.0;
	if (rotate < 0.0) rotate += 360.0;
}
double VectorProizvedenie(Coords a, Coords b, Coords c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
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
	double k = 1.5;
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	long long time_fps = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	long long time_count_fps = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	int STABLE_FPS = 80;
	long long fps = 0;
	Coords Camera = { 0.0, 0.0 };
	int mapa_width = rand() % 50 + 10;
	int mapa_height = rand() % 50 + 10;
	//std::vector<std::string> mapa;// = {
	//for (int y = 0; y < mapa_height; y++) {
	//	std::string s = "";
	//	for (int x = 0; x < mapa_width; x++) {
	//		if (rand() % 5) {
	//			s.push_back('O');
	//		}
	//		else {
	//			s.push_back('#');
	//		}
	//	}
	//	mapa.push_back(s);
	//}
	std::vector<Segment> WallSegments;
	//for (int y = 1; y < mapa_height; y++) {
	//	for (int x = 0; x < mapa_width; x++) {
	//		if (mapa[y][x] != mapa[y - 1][x]) {
	//			WallSegments.push_back({
	//				{ mapa_width / 2.0 - x , mapa_height / 2.0 - y },
	//				{ mapa_width / 2.0 - x - 1 , mapa_height / 2.0 - y},
	//				0, mapa_height / 2.0 - y,
	//				});
	//		}
	//	}
	//}

	//for (int x = 1; x < mapa[0].size(); x++) {
	//	for (int y = 0; y < mapa.size(); y++) {
	//		if (mapa[y][x] != mapa[y][x - 1]) {
	//			WallSegments.push_back({
	//				{ mapa_width / 2.0 - x , mapa.size() / 2.0 - y },
	//				{ mapa_width / 2.0 - x, mapa.size() / 2.0 - y - 1},
	//				999.0, 0,
	//				});
	//		}
	//	}
	//}

	for (int i = 0; i < 30; i++) {
		int x1 = mapa_width / 2.0 - rand() % mapa_width;
		int y1 = mapa_height / 2.0 - rand() % mapa_height;
		int x2 = mapa_width / 2.0 - rand() % mapa_width;
		int y2 = mapa_height / 2.0 - rand() % mapa_height;
		while (x1 == x2 || y1 == y2) {
			x1 = mapa_width / 2.0 - rand() % mapa_width;
			y1 = mapa_height / 2.0 - rand() % mapa_height;
		}
		double dx1 = x1 * 1.0f;
		double dy1 = y1 * 1.0f;
		double dx2 = x2 * 1.0f;
		double dy2 = y2 * 1.0f;
		WallSegments.push_back({ { dx1, dx2 }, { dy1, dy2 }, (dy1 - dy2) / (dx1 - dx2), dy1 - dx1 * (dy1 - dy2) / (dx1 - dx2) });
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		long long time_now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
		if (time_now - time_count_fps < 1000) fps++;
		else {
			time_count_fps = time_now;
			std::cout << "fps: " << fps << '\n';
			time_count_fps = time_now;
			fps = 0;
		}
		Coords OldCamera = Camera;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			Camera.y += (time_now - time) / 1000.0 * 3 * sin((rotate + 60.0) / 180.0 * M_PI);
			Camera.x += (time_now - time) / 1000.0 * 3 * cos((rotate + 60.0) / 180.0 * M_PI);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			Camera.y -= (time_now - time) / 1000.0 * 3 * sin((rotate + 60.0) / 180.0 * M_PI);
			Camera.x -= (time_now - time) / 1000.0 * 3 * cos((rotate + 60.0) / 180.0 * M_PI);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			Camera.y -= (time_now - time) / 1000.0 * 3 * sin((rotate + 60.0 + 90.0) / 180.0 * M_PI);
			Camera.x -= (time_now - time) / 1000.0 * 3 * cos((rotate + 60.0 + 90.0) / 180.0 * M_PI);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera.y -= (time_now - time) / 1000.0 * 3 * sin((rotate + 60.0 - 90.0) / 180.0 * M_PI);
			Camera.x -= (time_now - time) / 1000.0 * 3 * cos((rotate + 60.0 - 90.0) / 180.0 * M_PI);
		}
		glfwSetCursorPosCallback(window, mouse_callback);

		if (OldCamera.x != Camera.x) {
			for (auto el : WallSegments) {
				Coords M1 = OldCamera;
				Coords M2 = Camera;
				Coords P1 = el.a;
				Coords P2 = el.b;
				double d1 = VectorProizvedenie(M1, M2, P1);
				double d2 = VectorProizvedenie(M1, M2, P2);
				double d3 = VectorProizvedenie(M1, P1, P2);
				double d4 = VectorProizvedenie(M2, P1, P2);
				if (d1 * d2 < 0 && d3 * d4 < 0) {
					Camera.x = OldCamera.x;
					break;
				}
			}
		}
		if (OldCamera.y != Camera.y) {
			for (auto el : WallSegments) {
				Coords M1 = OldCamera;
				Coords M2 = Camera;
				Coords P1 = el.a;
				Coords P2 = el.b;
				double d1 = VectorProizvedenie(M1, M2, P1);
				double d2 = VectorProizvedenie(M1, M2, P2);
				double d3 = VectorProizvedenie(M1, P1, P2);
				double d4 = VectorProizvedenie(M2, P1, P2);
				if (d1 * d2 < 0 && d3 * d4 < 0) {
					Camera.y = OldCamera.y;
					break;
				}
			}
		}

		time = time_now;
		int n = width;

		for (int i = 0; i < n; i++) {
			double r = (rotate + i * (120.0 / n)) / 180.0 * M_PI;
			double m = 1e10;
			double cosr = cos(r);
			double sinr = sin(r);
			double tanr = tan(r);

			for (auto el : WallSegments) {
				bool d1 = tanr * (el.a.x - Camera.x) + Camera.y > el.a.y;
				bool d2 = tanr * (el.b.x - Camera.x) + Camera.y > el.b.y;
				if (d1 + d2 == 1) {
					if (el.b.x != el.a.x) {
						double elk = (el.b.y - el.a.y) / (el.b.x - el.a.x);
						double elb = el.a.y - el.a.x * elk;
						/*double elk = el.fkx;
						double elb = el.fb;*/
						double finalx = (elb - (Camera.y - tanr * Camera.x)) / (tanr - elk);
						if (finalx - Camera.x <= 0 && cosr <= 0 || finalx - Camera.x > 0 && cosr > 0) {
							m = std::min(m, sqrt(pow(elk * finalx + elb - Camera.y, 2) + pow(Camera.x - finalx, 2)));
						}
					}
					else {
						double finalx = el.a.x;
						if (finalx - Camera.x <= 0 && cosr <= 0 || finalx - Camera.x > 0 && cosr > 0) {
							double finaly = tanr * finalx + (Camera.y - tanr * Camera.x);
							if (finaly <= std::max(el.a.y, el.b.y) && finaly >= std::min(el.a.y, el.b.y)) {
								m = std::min(m, sqrt(pow(Camera.x - finalx, 2) + pow(Camera.y - finaly, 2)));
							}
						}
					}
					/*double elk = el.fkx;
					double elb = el.fb;
					double finalx = (elb - (Camera.y - tanr * Camera.x)) / (tanr - elk);
					if (finalx - Camera.x <= 0 && cosr <= 0 || finalx - Camera.x > 0 && cosr > 0) {
						m = std::min(m, sqrt(pow(elk * finalx + elb - Camera.y, 2) + pow(Camera.x - finalx, 2)));
					}*/
				}
			}
			if (m == 1e10) continue;
			//m *= cos((i * (120.0 / n) - 60.0) * M_PI / 180.0);
			double dk = 1.0 / m * k / 2.0;
			glBegin(GL_LINE_STRIP);
			glColor3f(dk, dk, dk);
			glVertex2f((i - n / 2.0) / (width / 2.0), -dk);
			glVertex2f((i - n / 2.0) / (width / 2.0), dk);
			glEnd();
		}
		/*while (time_fps - time < 1000 / STABLE_FPS) time_fps = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
		time_fps = time_now;*/


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}