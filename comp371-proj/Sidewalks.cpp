#include "stdafx.h"
#include "Sidewalks.h"
#include "Textures.h"
#include "World.h"

Sidewalks::Sidewalks(const World& world, std::vector<Area>& areas, std::size_t partitions) :
world(world), sidewalkGrid(partitions, std::vector<ModelPtr>(partitions))
{
	using namespace glutil;
	streetLampArray.reset(new ModelArray(glutil::Model("models/StreetLamp/streetlamp.obj")));
	streetLampArray->initialAttrib = 3;
	lampShader = glutil::Shader::fromFile("shaders/vertex_lamp.txt", "shaders/fragment_lamp.txt");

	typedef std::tuple<std::vector<Vertex>, std::vector<GLuint>, std::shared_ptr<Texture>> VertexIndexTuple;
	typedef std::map<GLuint, VertexIndexTuple> VertexIndexMap;
	std::vector<std::vector<VertexIndexMap>> vertexIndexMapGrid(partitions, std::vector<VertexIndexMap>(partitions));
	for (auto& area : areas){
		auto coord = world.areaCoordinate(area, partitions, partitions);
		auto texture = Textures::randomSidewalk();
		GLuint textureID = texture->getResourceID();
		auto& currentMap = vertexIndexMapGrid[coord.second][coord.first];
		std::get<2>(currentMap[textureID]) = texture;
		std::vector<Vertex>& vertices = std::get<0>(currentMap[textureID]);
		std::vector<GLuint>& indices = std::get<1>(currentMap[textureID]);
		auto topleft = area.getTopleft();
		auto bottomleft = area.getBottomleft();
		int xWidth = area.getXWidth();
		int zWidth = area.getZWidth();
		constructSidewalk(topleft, xWidth, zWidth, 1, vertices, indices);
		glm::mat4 lampMat;
		glm::mat4 tlMat = glm::translate(lampMat, glm::vec3{ topleft.x + 0.5f, topleft.y, topleft.z + 0.5f });
		glm::mat4 trMat = glm::translate(lampMat, glm::vec3{ topleft.x + area.getXWidth() - 0.5f, topleft.y, topleft.z + 0.5f });
		glm::mat4 blMat = glm::translate(lampMat, glm::vec3{ bottomleft.x + 0.5f, bottomleft.y, bottomleft.z - 0.5f });
		glm::mat4 brMat = glm::translate(lampMat, glm::vec3{ bottomleft.x + area.getXWidth() - 0.5f, bottomleft.y, bottomleft.z - 0.5f });
		glm::mat4 tMidMat = glm::translate(lampMat, glm::vec3{ topleft.x + area.getXWidth() / 2, topleft.y, topleft.z + 0.5f });
		glm::mat4 bMidMat = glm::translate(lampMat, glm::vec3{ bottomleft.x + area.getXWidth() / 2, bottomleft.y, bottomleft.z - 0.5f });
		glm::mat4 lMidMat = glm::translate(lampMat, glm::vec3{ topleft.x + 0.5f, topleft.y, topleft.z + area.getZWidth() / 2 });
		glm::mat4 rMidMat = glm::translate(lampMat, glm::vec3{ topleft.x + area.getXWidth() - 0.5f, topleft.y, topleft.z + area.getZWidth() / 2 });
		glm::vec3 scaleVec{ 0.1f, 0.1f, 0.1f };
		tlMat = glm::scale(tlMat, scaleVec);
		trMat = glm::scale(trMat, scaleVec);
		blMat = glm::scale(blMat, scaleVec);
		brMat = glm::scale(brMat, scaleVec);
		tMidMat = glm::scale(tMidMat, scaleVec);
		bMidMat = glm::scale(bMidMat, scaleVec);
		lMidMat = glm::scale(lMidMat, scaleVec);
		rMidMat = glm::scale(rMidMat, scaleVec);
		streetLampArray->instanceData.push_back(make_tuple(tlMat));
		streetLampArray->instanceData.push_back(make_tuple(trMat));
		streetLampArray->instanceData.push_back(make_tuple(blMat));
		streetLampArray->instanceData.push_back(make_tuple(brMat));
		streetLampArray->instanceData.push_back(make_tuple(tMidMat));
		streetLampArray->instanceData.push_back(make_tuple(bMidMat));
		streetLampArray->instanceData.push_back(make_tuple(lMidMat));
		streetLampArray->instanceData.push_back(make_tuple(rMidMat));
		area.setBottomleft(glm::vec3{ bottomleft.x + 1, bottomleft.y, bottomleft.z - 1 });
		area.setXWidth(xWidth - 2);
		area.setZWidth(zWidth - 2);
	}

	for (std::size_t row = 0; row < vertexIndexMapGrid.size(); row++){
		for (std::size_t col = 0; col < vertexIndexMapGrid[row].size(); col++){
			auto& currentMap = vertexIndexMapGrid[row][col];
			if (currentMap.empty()) continue;
			std::vector<Mesh> meshes;
			for (auto& VI : currentMap){
				auto& tuple = VI.second;
				meshes.push_back(Mesh(std::move(std::get<0>(tuple)), std::move(std::get<1>(tuple)), std::vector<std::shared_ptr<Texture>>{std::get<2>(tuple)}));
			}
			sidewalkGrid[row][col].reset(new glutil::Model(std::move(meshes)));
		}
	}

	streetLampArray->bufferData();
}

Sidewalks::~Sidewalks()
{
}

void Sidewalks::constructSidewalk(const glm::vec3& topleft, int xWidth, int zWidth, int depth, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices) const
{
	if (xWidth - 2 < depth || zWidth - 2 < depth)
		return;
	// Horizontal strip
	using namespace glutil;
	glm::vec3 bottomleft{ topleft.x, topleft.y, topleft.z + depth };
	glm::vec3 d1{ 1.0f, 0.0f, 0.0f };
	glm::vec3 d2{ 0.0f, 0.0f, -1.0f };
	glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, xWidth, depth, vertices, indices);
	// Rightmost vertical strip
	bottomleft.x += (xWidth - depth);
	d1 = glm::vec3{ 0.0f, 0.0f, 1.0f };
	d2 = glm::vec3{ 1.0f, 0.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, zWidth - 2 * depth, depth, vertices, indices);
	// Bottom strip
	bottomleft = glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth };
	d1 = glm::vec3{ 1.0f, 0.0f, 0.0f };
	d2 = glm::vec3{ 0.0f, 0.0f, -1.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, xWidth, depth, vertices, indices);
	// Leftmost vertical strip
	bottomleft.z -= depth;
	bottomleft.x += depth;
	d1 = glm::vec3{ 0.0f, 0.0f, -1.0f };
	d2 = glm::vec3{ -1.0f, 0.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, zWidth - 2 * depth, depth, vertices, indices);
}

void Sidewalks::update()
{
	lampShader->use();
	GLuint viewLoc = glGetUniformLocation(lampShader->getProgram(), "view");
	GLuint projLoc = glGetUniformLocation(lampShader->getProgram(), "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(world.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(world.getProj()));
}

void Sidewalks::draw()
{
	if (streetLampArray)
		streetLampArray->draw(*lampShader);
	for (const auto& row : sidewalkGrid){
		for (const auto& col : row){
			if (col)
				col->draw(world.getShader());
		}
	}
}
