#pragma once

struct Area{

	void subdivide(std::vector<Area>& subAreas, int minXWidth, int minZWidth, int depth) const;
	inline int area() const { return xWidth * zWidth; }
	inline glm::vec3 getTopleft() const{ return glm::vec3{ bottomleft.x, bottomleft.y, bottomleft.z - zWidth }; }
	inline glm::vec3 getBottomleft() const { return bottomleft; }
	inline glm::vec3 getCenter() const { return{ bottomleft.x + xWidth / 2, bottomleft.y, bottomleft.z - zWidth / 2 }; }
	inline int getXWidth() const { return xWidth; }
	inline int getZWidth() const { return zWidth; }
	inline void setBottomleft(const glm::vec3& bottomleft) { this->bottomleft = bottomleft; }
	inline void setXWidth(int xWidth) { this->xWidth = xWidth; }
	inline void setZWidth(int zWidth) { this->zWidth = zWidth; }
	Area(const glm::vec3& bottomleft, int xWidth, int zWidth);
	~Area();
private:
	void divideAlongX(std::vector<Area>& subareas, int minXWidth) const;
	void divideAlongZ(std::vector<Area>& subareas, int minZWidth) const;
	glm::vec3 bottomleft;

	int xWidth;
	int zWidth;

};
