#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

struct Coords {
	double x;
	double y;
};

struct Segment {
	Coords a;
	Coords b;
};

bool SortSegments(Segment A, Segment B);
double DotProduct(Coords a, Coords b, Coords c);
void MapInit();

extern int MapWidth;
extern int MapHeight;

extern std::vector<std::string> MapInStrings;
extern std::vector<Segment> MapSegments;

#endif
