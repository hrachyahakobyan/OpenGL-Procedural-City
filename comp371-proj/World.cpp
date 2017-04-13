#include "stdafx.h"
#include "World.h"
#include "Building.h"
#include "Ground.h"
#include "Random.h"
#include "Textures.h"

World::World() 
{
}

World::~World()
{
}

void World::initialize(int worldWidth, int worldHeight)
{
	bldShader = glutil::Shader::fromFile("shaders/vertex.txt", "shaders/fragment.txt");
	bldShader->use();
	lightPos = glm::vec3{0.0f, 50.0f, 0.0f };
	lightDirection = glm::normalize(glm::vec3{1.0f, 0.0f, 0.0f });

	GLint lightColorLoc = glGetUniformLocation(bldShader->getProgram(), "lightColor");
	lightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	this->worldWidth = worldWidth;
	this->worldHeight = worldHeight;
	this->maxHeight = 50;
	ground = Ground::makeGround(glm::vec3{ 0.0f, -0.1f, 0.0f }, worldWidth, worldHeight);
	std::vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	skybox = glutil::Skybox::make("textures\\skybox", faces);
	makeStreets();
	std::vector<Lane> lanes;
	for (std::size_t i = 0; i < 5; i++){
		auto l = vStreets[i].getLanes();
		lanes.insert(lanes.end(), l.begin(), l.end());
	}
	for (std::size_t i = 0; i < 5; i++){
		auto l = hStreets[i].getLanes();
		lanes.insert(lanes.end(), l.begin(), l.end());
	}
	vehicles.reset(new Vehicles(*this, lanes));
}

void World::makeStreets()
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Ground::makeStreets(glm::vec3{ 0.0f, 0.0f, 0.0f }, worldWidth, worldHeight,
		vertices, indices, vStreets, hStreets);
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomRoad()}));
	streets = std::shared_ptr<Model>(new Model(std::move(meshes)));
	makeAreas(vStreets, hStreets);
	fillAreas();
}

void World::makeAreas(const std::vector<Street>& verticalStreets, const std::vector<Street>& horizontalStreets)
{
	int zoffset = 0;
	for (std::size_t i = 0; i < horizontalStreets.size(); i++){
		int xoffset = 0;
		int areaZWidth = static_cast<int>(-horizontalStreets[i].bottomLeft.z) + zoffset;
		int areaBottomLeftZ = zoffset;
		for (std::size_t j = 0; j < verticalStreets.size(); j++){
			int areaXWidth = static_cast<int>(verticalStreets[j].bottomLeft.x) - xoffset;
			int areaBottomLeftX = xoffset;
			xoffset = static_cast<int>(verticalStreets[j].bottomLeft.x) + verticalStreets[j].width;
			if (areaXWidth > 0 || areaZWidth > 0){
				using namespace glutil;
				areas.push_back(Area(glm::vec3{ areaBottomLeftX, 0.0f, areaBottomLeftZ }, areaXWidth, areaZWidth));
			}
		}
		zoffset = static_cast<int>(horizontalStreets[i].bottomLeft.z) - horizontalStreets[i].width;
	}

	sidewalks.reset(new Sidewalks(*this, areas, 1));

	std::vector<Area> subAreas;
	glm::vec3 c = center();
	float distMax = glm::distance(glm::vec3{ 0.0f, 0.0f, 0.0f }, c);
	for (const auto area : areas){
		float dist = glm::distance(area.getBottomleft(), c);
		float prob = dist / distMax;
		bool shouldDivide = Random::dice(prob);
		if (shouldDivide)
			area.subdivide(subAreas, 3, 3, 1);
		else
			subAreas.push_back(area);
	}
	areas = std::move(subAreas);
}

void World::fillAreas()
{
	int max = 1;
	for (const auto& area : areas){
		if (max == 0) break;
		max--;
		fillArea(area);
	}
}

int World::calculateHeightForArea(const Area& area) const
{
	float dist = glm::distance(area.getBottomleft(), center());
	int noise = std::max(0, 2 * Random::normal(0, 3));
	return std::max(2, int(maxHeight * calculateHeightCoefficientForDistance(dist)) + noise);
}

float World::calculateHeightCoefficientForDistance(float dist) const
{
	float distMax = glm::distance(glm::vec3{ 0.0f, 0.0f, 0.0f }, center());
	return glm::max(glm::exp( -dist / distMax) - 1.0f / glm::e<float>(), 0.0f);
}

void World::fillArea(const Area& area)
{
	int height = calculateHeightForArea(area);
	BuildingType type = static_cast<BuildingType>(Random::random(0, 3));
	auto bld = Building::make(type, area.getTopleft(), area.getXWidth(), area.getZWidth(), height);
	if (bld)
		buildings.push_back(bld);
}

void World::update(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos)
{
	GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::mat4 rot = glm::rotate(glm::mat4(1.f), deltaTime / 10.0f, glm::vec3{ 0.0f, 0.0f, 1.0f });
	lightDirection = rot * glm::vec4(lightDirection, 1.0f);

	this->view = view;
	this->proj = proj;
	this->viewPos = viewPos;
	bldShader->use();
	GLint modelLoc = glGetUniformLocation(bldShader->getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(bldShader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(bldShader->getProgram(), "projection");
	glm::mat4 model;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	GLint viewPosLoc = glGetUniformLocation(bldShader->getProgram(), "viewPos");
	glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);
	GLint lightDirLoc = glGetUniformLocation(bldShader->getProgram(), "lightDirection");
	glUniform3f(lightDirLoc, lightDirection.x, lightDirection.y, lightDirection.z);

	vehicles->update();
	sidewalks->update();
}


void World::draw()
{
	if (ground){
		ground->draw(*(bldShader.get()));
	}
	if (streets){
		streets->draw(*(bldShader.get()));
	}
	for (const auto& bld : buildings){
		bld->draw(*(bldShader.get()));
	}
	if (skybox){
		skybox->draw(view, proj);
	}
	if (sidewalks){
		sidewalks->draw();
	}
	if (vehicles){
		vehicles->draw();
	}
}
