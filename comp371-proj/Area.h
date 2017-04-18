#pragma once

struct Volume;

struct Area{
	void subdivide(std::vector<Area>& subAreas, int minXWidth, int minZWidth, int depth) const;
	inline int area() const { return xWidth * zWidth; }
	inline glm::vec3 getTopleft() const{ return glm::vec3{ bottomleft.x, bottomleft.y, bottomleft.z - zWidth }; }
	inline glm::vec3 getBottomleft() const { return bottomleft; }
	inline glm::vec3 getCenter() const { return{ bottomleft.x + xWidth / 2, bottomleft.y, bottomleft.z - zWidth / 2 }; }
	inline glm::vec3 getBottomRight() const { return glm::vec3{ bottomleft.x + xWidth, bottomleft.y, bottomleft.z }; }
	inline glm::vec3 getTopRight() const {return glm::vec3{ bottomleft.x + xWidth, bottomleft.y, bottomleft.z - zWidth };
}
	inline int getXWidth() const { return xWidth; }
	inline int getZWidth() const { return zWidth; }
	inline void setBottomleft(const glm::vec3& bottomleft) { this->bottomleft = bottomleft; }
	inline void setXWidth(int xWidth) { this->xWidth = xWidth; }
	inline void setZWidth(int zWidth) { this->zWidth = zWidth; }
	void addVolume(const Volume& vol);
	bool contains(const glm::vec3& pt) const;
	Area(){}
	Area(const glm::vec3& bottomleft, int xWidth, int zWidth);
	~Area();
	static Area fromTopleft(const glm::vec3& topleft, int xWidth, int zWidth){
		return Area(glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth }, xWidth, zWidth);
	}
private:
	void divideAlongX(std::vector<Area>& subareas, int minXWidth) const;
	void divideAlongZ(std::vector<Area>& subareas, int minZWidth) const;
	glm::vec3 bottomleft;
	std::vector<Volume> volumes;
	int xWidth;
	int zWidth;
};

struct Volume
{
public:
	Volume(const Area&, float height);
	~Volume();

	inline const Area& getArea() const { return area; }
	inline float getHeight() const { return height; }
	bool contains(const glm::vec3& point) const;
private:
	Area area;
	float height;
};
