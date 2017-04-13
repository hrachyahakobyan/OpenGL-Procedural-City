#include "stdafx.h"
#include "Street.h"
#include "Random.h"

std::vector<Lane> Street::getLanes() const
{
	int left = std::max(width / 2, 1);
	int right = width - left;
	MovementDirection dir;
	glm::vec3 dd;
	if (this->direction == Vertical){
		dir = South;
		dd = glm::vec3{ 1.0f, 0.0f, 0.0f };
	}
	else {
		dir = East;
		dd = glm::vec3{ 0.0f, 0.0f, -1.0f };
	}
	std::vector<Lane> lanes;
	glm::vec3 bl = bottomLeft;
	for (int i = 0; i < left; i++){
		lanes.push_back(Lane(dir, bl, int(length)));
		bl += dd;
	}
	if (this->direction == Vertical){
		dir = North;
	}
	else {
		dir = West;
	}
	for (int i = 0; i < right; i++){
		lanes.push_back(Lane(dir, bl, int(length)));
		bl += dd;
	}
	return lanes;
}

glm::vec3 Lane::bottomMid() const
{
	if (dir == South || dir == North){
		return glm::vec3{ bottomLeft.x + 0.5, bottomLeft.y, bottomLeft.z };
	}
	else {
		return glm::vec3{ bottomLeft.x, bottomLeft.y, bottomLeft.z - 0.5 };
	}
}

glm::vec3 Lane::directionVector() const
{
	if (dir == South){
		return glm::vec3{ 0.0f, 0.0f, 1.0f };
	}
	else if(dir == North) {
		return glm::vec3{ 0.0f, 0.0f, -1.0f };
	}
	else if (dir == West){
		return glm::vec3{ -1.0f, 0.0f, 0.0f };
	} 
	return glm::vec3{ 1.0f, 0.0f, 0.0f };
}

std::vector<glm::vec3> Lane::getVehiclePositions(float vehicleLength, int numVehicles) const
{
	int actualNumvehicles = std::min(numVehicles, int(std::floor(length / vehicleLength)));
	std::vector<glm::vec3> vehicles(actualNumvehicles);
	if (actualNumvehicles == 0)
		return vehicles;
	glm::vec3 dir;
	float distance = (length - actualNumvehicles * vehicleLength) / (std::max(actualNumvehicles - 1, 1));
	glm::vec3 btmMid = bottomMid();
	bool vertical = (this->dir == South || this->dir == North);
	if (vertical){
		dir = glm::vec3{ 0.0f, 0.0f, -1.0f };
	}
	else {
		dir = glm::vec3{ 1.0, 0.0f, 0.0f };
	}
	vehicles[0] = btmMid + float(Random::random(0, length)) * dir;
	if (vertical)
		vehicles[0].z = glm::mod(vehicles[0].z, -float(length));
	else
		vehicles[0].x = glm::mod(vehicles[0].x, float(length));
	for (int i = 1; i < actualNumvehicles; i++){
		vehicles[i] = vehicles[i - 1] + distance * dir;
		if (vertical)
			vehicles[i].z = glm::mod(vehicles[i].z, -float(length));
		else
			vehicles[i].x = glm::mod(vehicles[i].x, float(length));
	}
	return vehicles;
}