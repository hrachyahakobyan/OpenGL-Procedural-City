#include "stdafx.h"
#include "Ground.h"


std::shared_ptr<glutil::Model> Ground::make(const glm::vec3& bottomleft, int width, int height)
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh::grid2D(bottomleft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, width, height, vertices, indices);
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices)));
	meshes.back().addTexture(Texture::fromFile("textures\\checker.png", GL_TEXTURE_2D, Diffusive));
	return std::shared_ptr<Model>(new Model(std::move(meshes)));
}


std::shared_ptr<glutil::Model> Ground::makeStreet(const glm::vec3& bottomleft, StreetDirection dir, int width, int height)
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	if (dir == Vertical)
		Mesh::grid2D(bottomleft, glm::vec3{1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, width, height, vertices, indices);
	else
		Mesh::grid2D(bottomleft, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, width, height, vertices, indices);
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices)));
	meshes.back().addTexture(Texture::fromFile("textures\\road2.jpg", GL_TEXTURE_2D, Diffusive));
	return std::shared_ptr<Model>(new Model(std::move(meshes)));
}

