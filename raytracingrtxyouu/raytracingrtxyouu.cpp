#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>

# define M_PI           3.14159265358979323846
struct WallFunction {
	char type;
	double main;
	double a;
	double b;
};
struct Coords {
	double x;
	double y;
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

int BinSearch(const std::vector<WallFunction>& funcs, double x) {
	auto it = std::lower_bound(funcs.begin(), funcs.end(), x,
		[](const WallFunction& f, double val) {
			return f.main < val;
		});
	
	if (it == funcs.begin()) return 0;
	if (it == funcs.end()) return funcs.size() - 1;
	return static_cast<int>(std::distance(funcs.begin(), it)) - 1;
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
	long long fps = 0;
	long long fps_for_screen = 0;
	Coords Camera = { 0.0, 0.0 };
	int mapa_width = rand() % 50 + 10;
	int mapa_height = rand() % 50 + 10;
	std::vector<std::string> mapa;// = {
	for (int y = 0; y < mapa_height; y++) {
		std::string s = "";
		for (int x = 0; x < mapa_width; x++) {
			if (rand() % 5) {
				s.push_back('O');
			}
			else {
				s.push_back('#');
			}
		}
		mapa.push_back(s);
	}
	std::vector<WallFunction> FuncWalls;
	std::vector<WallFunction> FuncWallsX;
	std::vector<WallFunction> FuncWallsY;
	for (int y = 1; y < mapa.size(); y++) {
		for (int x = 0; x < mapa[0].size(); x++) {
			if (mapa[y][x] != mapa[y - 1][x]) {
				FuncWalls.push_back(
					WallFunction{
					 'y', mapa.size() / 2.0 - y,
					 mapa[0].size() / 2.0 - x,
					 mapa[0].size() / 2.0 - x - 1,
					}
					);
				FuncWallsY.push_back(
					WallFunction{
					 'y', mapa.size() / 2.0 - y,
					 mapa[0].size() / 2.0 - x,
					 mapa[0].size() / 2.0 - x - 1,
					}
					);
			}
		}
	}

	for (int x = 1; x < mapa[0].size(); x++) {
		for (int y = 0; y < mapa.size(); y++) {
			if (mapa[y][x] != mapa[y][x - 1]) {
				FuncWalls.push_back(
					WallFunction{
					 'x', mapa[0].size() / 2.0 - x,
					 mapa.size() / 2.0 - y,
					 mapa.size() / 2.0 - y - 1,
					}
					);
				FuncWallsX.push_back(
					WallFunction{
					 'x', mapa[0].size() / 2.0 - x,
					 mapa.size() / 2.0 - y,
					 mapa.size() / 2.0 - y - 1,
					}
					);
			}
		}
	}
	std::sort(FuncWallsX.begin(), FuncWallsX.end(), [](WallFunction a, WallFunction b) {return a.main < b.main;});
	std::sort(FuncWallsY.begin(), FuncWallsY.end(), [](WallFunction a, WallFunction b) {return a.main < b.main;});


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		long long time_now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
		if (time_now - time_fps < 1000) fps++;
		else { 
			time_fps = time_now; 
			std::cout << "fps: " << fps << '\n'; 
			fps_for_screen = fps; 
			fps = 0;
		}

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

		time = time_now;
		int n = width;

		for (int i = 0; i < n; i++) {
			glBegin(GL_LINE_STRIP);
			double r = (rotate + i * (1 / (n / 120.0))) / 180.0 * M_PI;
			double m = 1e10;

			int l = BinSearch(FuncWallsX, Camera.x);
			for (int i = 0; i < FuncWallsX.size(); i++) {
				bool out = false;
				if (l + i < FuncWallsX.size()) {
					WallFunction el = FuncWallsX[l + i];
					if ((el.main - Camera.x) <= 0 && cos(r) <= 0 || (el.main - Camera.x) > 0 && cos(r) > 0) {
						double y = tan(r) * (el.main - Camera.x) + Camera.y;
						if (y <= std::max(el.a, el.b) && std::min(el.a, el.b) <= y) {
							double mn = sqrt(pow(y - Camera.y, 2) + pow(el.main - Camera.x, 2));
							m = std::min(mn, m);
							out = true;
						}
					}
				} 
				if (l - i >= 0) {
					WallFunction el = FuncWallsX[l - i];
					if ((el.main - Camera.x) <= 0 && cos(r) <= 0 || (el.main - Camera.x) > 0 && cos(r) > 0) {
						double y = tan(r) * (el.main - Camera.x) + Camera.y;
						if (y <= std::max(el.a, el.b) && std::min(el.a, el.b) <= y) {
							double mn = sqrt(pow(y - Camera.y, 2) + pow(el.main - Camera.x, 2));
							m = std::min(mn, m);
							out = true;
						}
					}
				}
				if (out) break;
			}

			l = BinSearch(FuncWallsY, Camera.y);
			for (int i = 0; i < FuncWallsX.size(); i++) {
				bool out = false;
				if (l + i < FuncWallsY.size()) {
					WallFunction el = FuncWallsY[l + i];
					if ((el.main - Camera.y) <= 0 && sin(r) <= 0 || (el.main - Camera.y) > 0 && sin(r) > 0) {
						double x = (el.main - Camera.y) / tan(r) + Camera.x;
						if (x <= std::max(el.a, el.b) && std::min(el.a, el.b) <= x) {
							double mn = sqrt(pow(x - Camera.x, 2) + pow(el.main - Camera.y, 2));
							m = std::min(mn, m);
							out = true;
						}
					}
				}
				if (l - i >= 0) {
					WallFunction el = FuncWallsY[l - i];
					if ((el.main - Camera.y) <= 0 && sin(r) <= 0 || (el.main - Camera.y) > 0 && sin(r) > 0) {
						double x = (el.main - Camera.y) / tan(r) + Camera.x;
						if (x <= std::max(el.a, el.b) && std::min(el.a, el.b) <= x) {
							double mn = sqrt(pow(x - Camera.x, 2) + pow(el.main - Camera.y, 2));
							m = std::min(mn, m);
							out = true;
						}
					}
				}
				if (out) break;
			}
			/*for (WallFunction el : FuncWalls) {
				if (el.type == 'y') {
					if ((el.main - Camera.y) <= 0 && sin(r) <= 0 || (el.main - Camera.y) > 0 && sin(r) > 0) {
						double x = (el.main - Camera.y) / tan(r) + Camera.x;
						if (x <= std::max(el.a, el.b) && std::min(el.a, el.b) <= x) {
							double mn = sqrt(pow(x - Camera.x, 2) + pow(el.main - Camera.y, 2));
							m = std::min(mn, m);
						}
					}
				}
				else if (el.type == 'x') {
					if ((el.main - Camera.x) <= 0 && cos(r) <= 0 || (el.main - Camera.x) > 0 && cos(r) > 0) {
						double y = tan(r) * (el.main - Camera.x) + Camera.y;
						if (y <= std::max(el.a, el.b) && std::min(el.a, el.b) <= y) {
							double mn = sqrt(pow(y - Camera.y, 2) + pow(el.main - Camera.x, 2));
							m = std::min(mn, m);
						}
					}
				}
			}*/
			if (m == 1e10) continue;
			double dk = 1.0 / m * k / 2.0;
			glColor3f(dk * 2.5, dk * 2.5, dk*2.5);
			glVertex2f((i - n / 2.0) / (width / 2.0), -dk);
			glVertex2f((i - n / 2.0) / (width / 2.0), dk);
			glEnd();
		}

		
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}