#pragma once

enum StreetDirection{
	Vertical, Horizontal
};

enum MovementDirection{
	South, North, West, East
};

/**
*	@brief struct to hold data of a single lane in a street
*	@author Hrachya Hakobyan
*/
struct Lane{
	MovementDirection dir;
	glm::vec3 bottomLeft;
	int length;
	Lane(MovementDirection d, const glm::vec3& bl, int length) : dir(d), bottomLeft(bl), length(length){}
	std::vector<glm::vec3> getVehiclePositions(float vehicleLength, int numVehicles) const;
	/**
	*	Get the bottom mid coordinate of the lane
	*	For horizontal lanes, it is the left mid point of the rectangle representing the lane
	*	For vertical lanes, it is the bottom mid point of the rectangle representing the lane
	*   @return a 3D point
	*/
	glm::vec3 bottomMid() const;
	glm::vec3 directionVector() const;
};


/**
*	@brief Struct to hold street data
*	@author Hrachya Hakobyan
*/
struct Street{
	StreetDirection direction;
	glm::vec3 bottomLeft;
	int width;
	float length;
	Street(StreetDirection d, const glm::vec3& bl, int w, float h) : direction(d), bottomLeft(bl), width(w), length(h){}
	/**
	*	Construct the lanes of the street
	*	@return a vector of lane objects
	*/
	std::vector<Lane> getLanes() const;
};