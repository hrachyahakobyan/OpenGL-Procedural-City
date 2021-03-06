#pragma once
#include "Area.h"

class World;

/**
*	@brief Handles the creation of sidewalks and all models contained on them
*	@author Hrachya Hakobyan
*/
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
	std::vector<std::vector<ModelPtr>> sidewalkGrid;
	void constructSidewalk(const glm::vec3& topleft, int xWidth, int zWidth, int depth, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices) const;
};

