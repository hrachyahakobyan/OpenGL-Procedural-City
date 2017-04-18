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
	fogColor = glm::vec3{ 0.5f, 0.5f, 0.5f };
	bldShader = glutil::Shader::fromFile("shaders/vertex.txt", "shaders/fragment.txt");
	bldShader->use();
	lightPos = glm::vec3{0.0f, 50.0f, 0.0f };
	lightDirection = glm::normalize(glm::vec3{5.0f, -0.5f, 0.0f });
	GLint lightColorLoc = glGetUniformLocation(bldShader->getProgram(), "lightColor");
	lightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	this->worldWidth = worldWidth;
	this->worldHeight = worldHeight;
	this->maxHeight = 70;
	ground = Ground::makeGround(glm::vec3{ 0.0f, -0.1f, 0.0f }, worldWidth, worldHeight);
	std::vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	//skybox = glutil::Skybox::make("textures\\skybox", faces);
	makeStreets();
	makeAreas(vStreets, hStreets);
	initializeAreaGrid();
	buildings.reset(new Building(*this, areas, 2));
	std::vector<Lane> lanes;
	for (std::size_t i = 0; i < vStreets.size() - 1; i++){
		auto l = vStreets[i].getLanes();
		lanes.insert(lanes.end(), l.begin(), l.end());
	}
	for (std::size_t i = 0; i < hStreets.size() - 1; i++){
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
			if (areaXWidth > 0 && areaZWidth > 0){
				using namespace glutil;
				areas.push_back(Area(glm::vec3{ areaBottomLeftX, 0.0f, areaBottomLeftZ }, areaXWidth, areaZWidth));
			}
		}
		zoffset = static_cast<int>(horizontalStreets[i].bottomLeft.z) - horizontalStreets[i].width;
	}

	sidewalks.reset(new Sidewalks(*this, areas, 1));

	std::vector<Area> subAreas;
	glm::vec3 c = center();
	int depth = 2;
	float distMax = glm::distance(glm::vec3{ 0.0f, 0.0f, 0.0f }, c);
	while (depth-- > 0){
		for (const auto area : areas){
			float dist = glm::distance(area.getBottomleft(), c);
			float prob = dist / distMax;
			area.subdivide(subAreas, 6, 6);
		}
		areas = std::move(subAreas);
	}
}

std::pair<std::size_t, std::size_t> World::areaCoordinate(const Area& a, std::size_t width, std::size_t height) const
{
	float gridWidth = float(worldWidth) / width;
	float gridHeight = float(worldHeight) / height;
	glm::vec3 areaCenter = a.getCenter();
	std::size_t xCoord = std::size_t(std::floor(areaCenter.x / gridWidth));
	std::size_t yCoord = std::size_t(std::floor((worldHeight - std::abs(areaCenter.z)) / gridHeight));
	xCoord = std::min(xCoord, width - 1);
	yCoord = std::min(yCoord, height - 1);
	return std::make_pair(xCoord, yCoord);

}

void World::update(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos)
{
	GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	glm::mat4 rot = glm::rotate(glm::mat4(1.f), deltaTime / 2.0f, glm::vec3{ 0.0f, 0.0f, 1.0f });
	lightDirection = rot * glm::vec4(lightDirection, 1.0f);
	lightDirection = glm::normalize(lightDirection);
	float dot = glm::dot(-lightDirection, glm::vec3{ 0.0f, 1.0f, 0.0 });
	lightColor = glm::vec3(std::max(dot, 0.1f));

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
	GLint lightColorLoc = glGetUniformLocation(bldShader->getProgram(), "lightColor");
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	vehicles->update();
	sidewalks->update();
	buildings->update();
}

void World::draw()
{
	if (ground){
		ground->draw(*(bldShader.get()));
	}
	if (streets){
		streets->draw(*(bldShader.get()));
	}
	if (buildings){
		buildings->draw();
	}
	if (skybox){
		//skybox->draw(view, proj);
	}
	if (sidewalks){
		sidewalks->draw();
	}
	if (vehicles){
		vehicles->draw();
	}
}

bool World::intersectsPoint(const glm::vec3& point) const
{
	bool contains = false;
	auto coord = gridCoordinate(point, contains);
	if (!contains) return false;
	// Check against the ground
	if (point.y <= 0.1f) return true;
	for (auto areaPtr : areaGrid[coord.first][coord.second]){
		if (areaPtr->contains(point)) return true;
	}
	return false;
}

void World::initializeAreaGrid()
{
	gridPartitions = 16;
	const float partitionWidth = float(worldWidth) / gridPartitions;
	const float partitionHeight = float(worldHeight) / gridPartitions;
	areaGrid = std::vector<std::vector<std::vector<const Area*>>>(gridPartitions, std::vector<std::vector<const Area*>>(gridPartitions));

	for (const auto& area : areas){
		bool contains = false;
		auto coordsTopleft = gridCoordinate(area.getTopleft(), contains);
		assert(contains && "Area outside grid");
		auto coordsBottomright = gridCoordinate(area.getBottomRight(), contains);
		assert(contains && "Area outside grid");
		std::size_t iStart = coordsTopleft.first;
		std::size_t iEnd = coordsBottomright.first;
		std::size_t jStart = coordsTopleft.second;
		std::size_t jEnd = coordsBottomright.second;
		for (std::size_t i = iStart; i <= iEnd; i++){
			for (std::size_t j = jStart; j <= jEnd; j++){
				float partitionLeft = j * partitionWidth;
				float partitionRight = partitionLeft + partitionWidth;
				float partitionTop = (gridPartitions - i) * partitionHeight * -1.0f;
				float partitionBottom = partitionTop + partitionHeight;
				const glm::vec3 areaTopleft = area.getTopleft();
				float areaLeft = areaTopleft.x;
				float areaRight = areaTopleft.x + area.getXWidth();
				float areaTop = areaTopleft.z;
				float areaBottom = areaTopleft.z + area.getZWidth();
				bool overlap = !(areaLeft > partitionRight ||
								 areaRight < partitionLeft ||
								 areaTop > partitionBottom ||
								 areaBottom < partitionTop);
				if (overlap){
					areaGrid[i][j].push_back(&area);
				}
			}
		}
	}
}

std::pair<std::size_t, std::size_t> World::gridCoordinate(const glm::vec3& vec, bool& contains) const{
	std::pair<std::size_t, std::size_t> res;
	if (vec.x < 0 || vec.x > worldWidth || vec.z > 0 || vec.z < -worldHeight) {
		contains = false;
		return res;
	}
	const float partitionWidth = float(worldWidth) / gridPartitions;
	const float partitionHeight = float(worldHeight) / gridPartitions;
	contains = true;
	res.first = std::min(std::size_t(std::floor((worldHeight + vec.z) / partitionHeight)), gridPartitions - 1);
	res.second = std::min(std::size_t(std::floor(vec.x / partitionWidth)), gridPartitions - 1);
	return res;
}