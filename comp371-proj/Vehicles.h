#pragma once
#include "Street.h"

class World;

/**
*	@brief class Handles the creation, movement and rendering of vehicle objects in the world
*	@author Hrachya Hakobyan
*/
class Vehicles
{
public:
	Vehicles(const World& worldRef, const std::vector<Lane>& lanes);
	~Vehicles();
	void update();
	void draw();
private:
	typedef glutil::ModelArray<glutil::Model, glm::mat4> ModelArray;
	typedef tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<MovementDirection>> VehicleData;
	std::shared_ptr<glutil::Shader> shader;
	VehicleData vehicleData;
	std::vector<ModelArray> modelArrays;
	std::vector<float> speedErrors;
	std::vector<float> speeds;
	std::vector<int> sign;
	const World& world;
};

