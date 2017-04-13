#pragma once

enum StreetDirection{
	Vertical, Horizontal
};

enum MovementDirection{
	South, North, West, East
};

struct Lane{
	MovementDirection dir;
	glm::vec3 bottomLeft;
	int length;
	Lane(MovementDirection d, const glm::vec3& bl, int length) : dir(d), bottomLeft(bl), length(length){}
	std::vector<glm::vec3> getVehiclePositions(float vehicleLength, int numVehicles) const;
	glm::vec3 bottomMid() const;
	glm::vec3 directionVector() const;
};

struct Street{
	StreetDirection direction;
	glm::vec3 bottomLeft;
	int width;
	float length;
	Street(StreetDirection d, const glm::vec3& bl, int w, float h) : direction(d), bottomLeft(bl), width(w), length(h){}
	std::vector<Lane> getLanes() const;
};