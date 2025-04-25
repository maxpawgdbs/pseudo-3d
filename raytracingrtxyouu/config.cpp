#include <chrono>
#include "map.h"
#include "player.h"

double WallHeightCoefficient = 1.5;
long long TimeLastFrame = std::chrono::duration_cast<std::chrono::milliseconds>(
	std::chrono::system_clock::now().time_since_epoch()
).count();
long long TimeControlFps = std::chrono::duration_cast<std::chrono::milliseconds>(
	std::chrono::system_clock::now().time_since_epoch()
).count();
long long TimeCountFps = std::chrono::duration_cast<std::chrono::milliseconds>(
	std::chrono::system_clock::now().time_since_epoch()
).count();
int STABLE_FPS = 80;
int ScreenWidth = 1920;
int ScreenHeight = 1080;
