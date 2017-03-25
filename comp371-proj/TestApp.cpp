#include "stdafx.h"
#include "TestApp.h"
#include "Building.h"
#include "Ground.h"
#include "Random.h"

TestApp::TestApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

TestApp::TestApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

TestApp::~TestApp()
{
	glutil::Texture::purge();
}

void TestApp::initialize()
{
	glutil::ApplicationBase::initialize();
	glCullFace(GL_BACK);
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	shader.reset(new glutil::Shader("shaders/vertex.txt", "shaders/fragment.txt"));
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 30.f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)));
	camera->setMovementSpeed(10.f);
	ground = Ground::make(glm::vec3{ 0.0f, -0.1f, 0.0f }, 128, 128);
	makeStreets();
}

void TestApp::makeStreets()
{
	const int worldWidth = 128;
	const int worldHeight = 128;
	std::vector<std::pair<int, int>> verticalStreets;
	int left = worldWidth;
	while (left > 0){
		int skip = 5 + Random::random(0, 10);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 1 + std::max(0, Random::normal(3, 5));
		if (left < streetWidth)
			break;
		streets.push_back(Ground::makeStreet(glm::vec3{ worldWidth - left, 0.0f, 0 }, Vertical, streetWidth, worldHeight));
		verticalStreets.push_back(std::make_pair(worldWidth - left, worldWidth - left + streetWidth));
		left -= streetWidth;		
	}
	verticalStreets.push_back(std::make_pair(worldWidth, worldWidth));

	std::vector<std::pair<int, int>> horizontalStreets;
	left = worldHeight;
	while (left > 0){
		int skip = 10 + Random::random(0, 20);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 1 + std::max(0, Random::random(3, 5));
		if (left < streetWidth)
			break;
		streets.push_back(Ground::makeStreet(glm::vec3{ 0, 0.0f, -worldHeight +left }, Horizontal, streetWidth, worldWidth));
		horizontalStreets.push_back(std::make_pair(-worldHeight + left, -worldHeight + left - streetWidth));
		left -= streetWidth;
	}
	horizontalStreets.push_back(std::make_pair(-worldHeight, -worldHeight));

	int zoffset = 0;
	for (std::size_t i = 0; i < horizontalStreets.size(); i++){
		int xoffset = 0;
		int areaZWidth = -horizontalStreets[i].first + zoffset;
		int areaBottomLeftZ = zoffset;
		for (std::size_t j = 0; j < verticalStreets.size(); j++){
			int areaXWidth = verticalStreets[j].first - xoffset;
			int areaBottomLeftX = xoffset;
			xoffset = verticalStreets[j].second;
			if (areaXWidth > 0 || areaZWidth > 0){
				using namespace glutil;
				areas.push_back(Area(glm::vec3{ areaBottomLeftX, 0.0f, areaBottomLeftZ - areaZWidth }, areaXWidth, areaZWidth));
			}
		}
		zoffset = horizontalStreets[i].second;
	}

	int max = 30;
	for (const auto& area : areas){
		max--;
		if (max == 0) return;
		int height = 10 + Random::normal(10, 
			5);
		BuildingType type = static_cast<BuildingType>(Random::random(0, 1));
		buildings.push_back(Building::make(type, area.bottomleft, area.xWidth, area.zWidth, height));
	}
}

void TestApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	shader->use();
	glm::mat4 view = camera->view();
	// Projection 
	glm::mat4 projection = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 75.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void TestApp::render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (ground){
		ground->draw(*(shader.get()));
	}
	for (const auto& road : streets){
		road->draw(*(shader.get()));
	}
	for (const auto& bld : buildings){
		bld->draw(*(shader.get()));
	}
}

void TestApp::updateMovement()
{
	if (keys[GLFW_KEY_W])
		camera->move(glutil::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->move(glutil::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->move(glutil::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->move(glutil::RIGHT, deltaTime);
}

