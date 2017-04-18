#include "stdafx.h"
#include "Ground.h"
#include "Random.h"


std::shared_ptr<glutil::Model> Ground::makeGround(const glm::vec3& bottomleft, int width, int height)
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh::grid2D(glm::vec3{ -100.f, -0.1f, 100.f }, glm::vec3{ 50.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -50.f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 500.f, 500.f, vertices, indices);
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices)));
	meshes.back().addTexture(Texture::fromFile("textures\\road1.jpg", GL_TEXTURE_2D, Diffusive));
	return std::shared_ptr<Model>(new Model(std::move(meshes)));
}

void Ground::makeStreets(const glm::vec3& bottomleft, int worldWidth, int worldHeight,
	std::vector<glutil::Vertex>& v, std::vector<GLuint>& i,
	std::vector<Street>& vertical,
	std::vector<Street>& horizontal)
{
	int left = worldWidth;
	float wHeight = float(worldHeight);
	float wWidth = float(worldWidth);
	while (left > 0){
		int skip = 15 + Random::random(0, 10);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 2 + std::max(0, Random::normal(1, 4));
		if (left < streetWidth)
			break;
		Street street(Vertical, glm::vec3{ bottomleft.x + wWidth - left, bottomleft.y, bottomleft.z }, streetWidth, wHeight);
		Ground::makeStreet(street, v, i);
		vertical.push_back(street);
		left -= streetWidth;
	}
	vertical.push_back(Street(Vertical, glm::vec3{ bottomleft.x + wWidth, bottomleft.y, bottomleft.z },
		0, wHeight));
	left = worldHeight;
	while (left > 0){
		int skip = 15 + Random::random(0, 10);
		if (left < skip)
			break;
		left -= skip;
		int streetWidth = 2 + std::max(0, Random::normal(1, 4));
		if (left < streetWidth)
			break;
		Street street(Horizontal, glm::vec3{ bottomleft.x, bottomleft.y, bottomleft.z - wHeight + left }, streetWidth, wWidth);
		Ground::makeStreet(street, v, i);
		horizontal.push_back(street);
		left -= streetWidth;
	}
	horizontal.push_back(Street(Horizontal, glm::vec3{ bottomleft.x, bottomleft.y, bottomleft.z - wHeight }, 0, wWidth));
}

void Ground::makeStreet(const Street& street, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices)
{
	using namespace glutil;
	if (street.direction == Vertical)
		Mesh::grid2D(street.bottomLeft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, street.width, street.length, vertices, indices);
	else
		Mesh::grid2D(street.bottomLeft, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, street.width, street.length, vertices, indices);
}

