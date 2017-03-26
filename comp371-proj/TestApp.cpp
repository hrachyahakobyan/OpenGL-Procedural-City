#include "stdafx.h"
#include "TestApp.h"
#include "Building.h"
#include "Ground.h"
#include "Random.h"
#include "Textures.h"

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
	shader = glutil::Shader::fromFile("shaders/vertex.txt", "shaders/fragment.txt");
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 30.f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)));
	camera->setMovementSpeed(10.f);
	ground = Ground::make(glm::vec3{ 0.0f, -0.1f, 0.0f }, 128, 128);
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
}

void TestApp::makeStreets()
{
	const int worldWidth = 128;
	const int worldHeight = 128;
	std::vector<std::pair<int, int>> verticalStreets;
	int left = worldWidth;
	while (left > 0){
		int skip = 6 + Random::random(0, 7);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 1 + std::max(0, Random::normal(0, 4));
		if (left < streetWidth)
			break;
		streets.push_back(Ground::makeStreet(glm::vec3{ worldWidth - left, 0.0f, 0.0f }, Vertical, streetWidth, worldHeight));
		verticalStreets.push_back(std::make_pair(worldWidth - left, worldWidth - left + streetWidth));
		left -= streetWidth;		
	}
	verticalStreets.push_back(std::make_pair(worldWidth, worldWidth));

	std::vector<std::pair<int, int>> horizontalStreets;
	left = worldHeight;
	while (left > 0){
		int skip = 6 + Random::random(0, 7);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 1 + std::max(0, Random::normal(0, 4));
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
				areas.push_back(Area(glm::vec3{ areaBottomLeftX, 0.0f, areaBottomLeftZ}, areaXWidth, areaZWidth));
			}
		}
		zoffset = horizontalStreets[i].second;
	}
	int max = 5;
	for (const auto& area : areas){
		if (max == 0) break;
		max--;
		fillArea(area);
	}
	makeSidewalks();
}

void TestApp::makeSidewalks()
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	for (const auto& area : areas){
		Building::constructSidewalk(area.topleft, area.xWidth, area.zWidth, 1, vertices, indices);
	}
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::sidewalk()}));
	sidewalks = std::shared_ptr<Model>(new Model(std::move(meshes)));
}

void TestApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	shader->use();
	view = camera->view();
	// Projection 
	proj = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 75.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
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
	if (skybox){
		skybox->draw(view, proj);
	}
	if (sidewalks){
		sidewalks->draw(*(shader.get()));
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

void TestApp::fillArea(const Area& area)
{
	int height = 10 + Random::normal(10,
		5);
	BuildingType type = static_cast<BuildingType>(Random::random(0, 2));
	auto bld = Building::make(type, glm::vec3{area.topleft.x + 1, area.topleft.y, area.topleft.z + 1}, area.xWidth - 2, area.zWidth - 2, height);
	if (bld)
		buildings.push_back(bld);
}

