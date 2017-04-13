#include "stdafx.h"
#include "Area.h"
#include "Random.h"

Area::Area(const glm::vec3& bottomleft, int xWidth, int zWidth) :
bottomleft(bottomleft),
xWidth(xWidth),
zWidth(zWidth)
{
}


Area::~Area()
{
}


void Area::subdivide(std::vector<Area>& subAreas, int minXWidth, int minZWidth, int depth) const
{
	if (depth <= 0) return;
	if (xWidth < 2 * minXWidth || zWidth < 2 * minZWidth) return;
	if (xWidth >= 2 * minXWidth){
		std::vector<Area> temp;
		divideAlongX(temp, minXWidth);
		if (zWidth >= 2 * minZWidth){
			for (const auto a : temp){
				a.divideAlongZ(subAreas, minZWidth);
			}
		}
		else {
			subAreas.insert(subAreas.end(), temp.begin(), temp.end());
		}
	}
	else if (zWidth >= 2 * minZWidth){
		std::vector<Area> temp;
		divideAlongZ(temp, minZWidth);
		if (xWidth >= 2 * minXWidth){
			auto areas = std::move(subAreas);
			for (const auto a : temp){
				a.divideAlongX(subAreas, minXWidth);
			}
		}
		else {
			subAreas.insert(subAreas.end(), temp.begin(), temp.end());
		}
	}
	else {
		subAreas.push_back(*this);
	}
	for (const auto a : subAreas){
		a.subdivide(subAreas, minXWidth, minZWidth, --depth);
	}
}

void Area::divideAlongX(std::vector<Area>& subareas, int minXWidth) const
{
	int minX = minXWidth;
	int maxX = xWidth - minXWidth;
	int xCut = Random::random(minX, maxX);
	subareas.push_back(Area(bottomleft, xCut, zWidth));
	glm::vec3 rightAreaBottomLeft{ bottomleft.x + xCut, bottomleft.y, bottomleft.z };
	int rightAreaXWidth = xWidth - xCut;
	if (minXWidth * 2 < xWidth && Random::random(0, 1)){
		rightAreaBottomLeft.x += 1;
		rightAreaXWidth -= 1;
	}
	subareas.push_back(Area(rightAreaBottomLeft, rightAreaXWidth, zWidth));
}

void Area::divideAlongZ(std::vector<Area>& subareas, int minZWidth) const
{
	int minZ = minZWidth;
	int maxZ = zWidth - minZWidth;
	int zCut = Random::random(minZ, maxZ);
	subareas.push_back(Area(bottomleft, xWidth, zCut));
	glm::vec3 topAreaBottomleft{ bottomleft.x, bottomleft.y, bottomleft.z - zCut };
	int topAreaZWidth = zWidth - zCut;
	if (minZWidth * 2 < zWidth && Random::random(0, 1)){
		topAreaBottomleft.z -= 1;
		topAreaZWidth -= 1;
	}
	subareas.push_back(Area(topAreaBottomleft, xWidth, topAreaZWidth));

}