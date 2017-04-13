#pragma once
#include "Street.h"

class Ground
{
public:
	static std::shared_ptr<glutil::Model> makeGround(const glm::vec3& bottomleft, int width, int height);
	static void makeStreets(const glm::vec3& bottomleft, int worldWidth, int worldHeight,
							std::vector<glutil::Vertex>& v, std::vector<GLuint>& i,
							std::vector<Street>& vertical,
							std::vector<Street>& horizontal);
	static void makeStreet(const Street& s, std::vector<glutil::Vertex>& vertices,
							std::vector<GLuint>& indices);
};

