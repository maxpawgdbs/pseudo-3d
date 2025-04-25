#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include "map.h"

bool SortSegments(Segment A, Segment B) {
	if (A.a.x != B.a.x) return A.a.x < B.a.x;
	else if (A.b.x != B.b.x) return A.b.x < B.b.x;
	else if (A.a.y != B.a.y) return A.a.y < B.a.y;
	else if (A.b.y != B.b.y) return A.b.y < B.b.y;
}

double DotProduct(Coords a, Coords b, Coords c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int MapWidth;
int MapHeight;

std::vector<std::string> MapInStrings;
std::vector<Segment> MapSegments;

void MapInit() {
	MapWidth = rand() % 1 + 5;
	MapHeight = rand() % 1 + 5;
	for (int y = 0; y < MapHeight; y++) {
		std::string s;
		for (int x = 0; x < MapWidth; x++) {
			if (rand() % 5) s.push_back('O');
			else s.push_back('#');
		}
		MapInStrings.push_back(s);
	}
	for (int y = 1; y < MapHeight; y++) {
		for (int x = 0; x < MapWidth; x++) {
			if (MapInStrings[y][x] != MapInStrings[y - 1][x]) {
				MapSegments.push_back({
					{MapWidth / 2.0 - x, MapHeight / 2.0 - y},
					{MapWidth / 2.0 - x - 1, MapHeight / 2.0 - y},
					}
				);
			}
		}
	}
	for (int x = 1; x < MapWidth; x++) {
		for (int y = 0; y < MapHeight; y++) {
			if (MapInStrings[y][x] != MapInStrings[y - 1][x]) {
				MapSegments.push_back(
					{
						{MapWidth / 2.0 - x, MapHeight / 2.0 - y},
						{MapWidth / 2.0 - x, MapHeight / 2.0 - y - 1},
					}
				);
			}
		}
	}
	std::sort(MapSegments.begin(), MapSegments.end(), SortSegments);
	//MapSegments
}
