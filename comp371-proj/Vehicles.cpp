#include "stdafx.h"
#include "Vehicles.h"
#include "Random.h"
#include "World.h"

Vehicles::Vehicles(const World& world, const std::vector<Lane>& lanes) : 
world(world)
{
	if (lanes.empty())
		return;
	modelArrays = std::vector<ModelArray>{ glutil::Model("models/Taxi/Taxi.obj"),
		glutil::Model("models/Car/Car.obj"),
		glutil::Model("models/Bus/bus.obj") };
	int vehiclesTotal = 100;
	int vehiclesPerLane = std::max(int(vehiclesTotal / lanes.size()), 1);
	for (const auto& lane : lanes){
		std::vector<glm::vec3> positions = lane.getVehiclePositions(3.0f, vehiclesPerLane);
		get<1>(vehicleData).insert(get<1>(vehicleData).end(), positions.size(), lane.directionVector());
		vehicleData.first.insert(vehicleData.first.end(), positions.begin(), positions.end());
		get<2>(vehicleData).insert(get<2>(vehicleData).end(), positions.size(), lane.dir);
	}
	for (std::size_t i = get<1>(vehicleData).size() - 1; i > 0; i--)
	{
		std::size_t j = Random::random(0, i);
		std::swap(get<1>(vehicleData)[i], get<1>(vehicleData)[j]);
		std::swap(get<2>(vehicleData)[i], get<2>(vehicleData)[j]);
		std::swap(get<0>(vehicleData)[i], get<0>(vehicleData)[j]);
	}
	speedErrors = std::vector<float>(vehicleData.first.size());
	double busProb = 0.3f;
	for (int i = 0; i < vehicleData.first.size(); i++){
		double isBus = Random::dice(busProb);
		double isTaxi = (!isBus && Random::dice(0.5f));
		ModelArray* arr = &modelArrays[0];
		if (isBus)
			arr = &modelArrays[2];
		else if (isTaxi)
			arr = &modelArrays[1];
		arr->instanceData.push_back(tuple<glm::mat4>());
		speedErrors[i] = Random::random(-100, 100) / 100.f;
	}
	modelArrays[0].initialAttrib = 3;
	modelArrays[1].initialAttrib = 3;
	modelArrays[2].initialAttrib = 3;
	speeds = std::vector<float>{1.7f, 2.4f, 1.3f};
	sign = std::vector<int>{1, 1, -1};
	shader = glutil::Shader::fromFile("shaders/vertex_vehicle.txt", "shaders/fragment_vehicle.txt");
	shader->use();
	GLint lightColorLoc = glGetUniformLocation(shader->getProgram(), "lightColor");
	glUniform3f(lightColorLoc, world.getLightColor().x, world.getLightColor().y, world.getLightColor().z);
}

Vehicles::~Vehicles()
{
}

void Vehicles::update()
{
	shader->use();
	GLuint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLuint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(world.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(world.getProj()));
	GLint viewPosLoc = glGetUniformLocation(shader->getProgram(), "viewPos");
	glUniform3f(viewPosLoc, world.getViewPos().x, world.getViewPos().y, world.getViewPos().z);
	GLint lightDirLoc = glGetUniformLocation(shader->getProgram(), "lightDirection");
	glUniform3f(lightDirLoc, world.getLightDirection().x, world.getLightDirection().y, world.getLightDirection().z);
	GLint lightColorLoc = glGetUniformLocation(shader->getProgram(), "lightColor");
	glUniform3f(lightColorLoc, world.getLightColor().x, world.getLightColor().y, world.getLightColor().z);
	
	std::size_t index = 0;
	for (std::size_t i = 0; i < modelArrays.size(); i++){
		float speed = speeds[i] + speedErrors[index];
		for (auto& tup : modelArrays[i].instanceData){
			MovementDirection dir = get<2>(vehicleData)[index];
			vehicleData.first[index] += get<1>(vehicleData)[index] * speed * world.getDeltaTime();
			vehicleData.first[index].x = glm::mod(vehicleData.first[index].x, float(world.getWorldWidth()));
			vehicleData.first[index].z = glm::mod(vehicleData.first[index].z, -float(world.getWorldHeight()));
			glm::mat4 m;
			m = glm::translate(m, vehicleData.first[index]);
			if (dir == South){
				if (i != 2)
					m = glm::rotate(m, glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
			}
			else if (dir == West)
				m = glm::rotate(m, sign[i] * glm::half_pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
			else if (dir == East)
				m = glm::rotate(m, sign[i] * -glm::half_pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
			else{
				if (i == 2)
					m = glm::rotate(m, -glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
			}
			float scaleFactor = (i == 2 ? 0.15f : 0.007f);
			m = glm::scale(m, glm::vec3{ scaleFactor, scaleFactor, scaleFactor });
			tup.first = m;
			index++;
		}
		modelArrays[i].bufferData();
	}
}

void Vehicles::draw()
{
	for (auto& arr : modelArrays)
		arr.draw(*(shader.get()));
}
