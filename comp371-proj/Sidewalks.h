#pragma once
#include "Area.h"

class World;

class Sidewalks
{
public:
	Sidewalks(const World& world, std::vector<Area>&);
	~Sidewalks();
	void update();
	void draw();
private:
	const World& world;
	typedef glutil::ModelArray<glutil::Model, glm::mat4> ModelArray;
	typedef std::shared_ptr<glutil::Shader> Shader;
	Shader lampShader;
	std::unique_ptr<ModelArray> streetLampArray;
	std::map<GLuint, std::shared_ptr<glutil::Model>> sidewalks;
	void constructSidewalk(const glm::vec3& topleft, int xWidth, int zWidth, int depth, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices) const;
};

