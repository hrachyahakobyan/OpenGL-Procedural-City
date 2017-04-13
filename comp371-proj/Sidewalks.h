#pragma once
#include "Area.h"

class World;

class Sidewalks
{
public:
	Sidewalks(const World& world, std::vector<Area>&, std::size_t partitions);
	~Sidewalks();
	void update();
	void draw();
private:
	const World& world;
	typedef glutil::ModelArray<glutil::Model, glm::mat4> ModelArray;
	typedef std::shared_ptr<glutil::Shader> Shader;
	typedef std::shared_ptr<glutil::Model> ModelPtr;
	Shader lampShader;
	std::unique_ptr<ModelArray> streetLampArray;
	std::unique_ptr<glutil::Model> sidewalks;
	std::vector<std::vector<ModelPtr>> sidewalkGrid;
	std::pair<std::size_t, std::size_t> areaCoordinate(const Area& a) const;
	void constructSidewalk(const glm::vec3& topleft, int xWidth, int zWidth, int depth, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices) const;
};

