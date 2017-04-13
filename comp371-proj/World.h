#pragma once
#include "Area.h"
#include "Ground.h"
#include "Vehicles.h"
#include "Sidewalks.h"

class World
{
public:
	World();
	~World();
	void initialize(int worldWidth, int worldHeight);
	void update(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);
	void draw();

	inline const glm::mat4& getView() const { return view; }
	inline const glm::mat4& getProj() const { return proj; }
	inline const glm::vec3& getViewPos() const { return viewPos; }
	inline int getWorldHeight() const { return worldHeight; }
	inline int getWorldWidth() const { return worldWidth; }
	inline const glm::vec3& getLightPos() const { return lightPos; }
	inline const glm::vec3& getLightColor() const { return lightColor; }
	inline const glm::vec3& getLightDirection() const { return lightDirection; }
	inline const glutil::Shader& getShader() const { return *bldShader; }
	inline const GLfloat getDeltaTime() const { return deltaTime; }
private:
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 viewPos;
	int maxHeight;
	int worldWidth;
	int worldHeight;
	glm::vec3 lightPos;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	std::unique_ptr<Vehicles> vehicles;
	std::unique_ptr<Sidewalks> sidewalks;
	std::shared_ptr<glutil::Shader> bldShader;
	std::shared_ptr<glutil::Model> ground;
	std::shared_ptr<glutil::Model> streets;
	std::vector<std::shared_ptr<glutil::Model>> buildings;
	std::shared_ptr<glutil::Skybox> skybox;
	std::vector<Area> areas;
	std::vector<Street> vStreets;
	std::vector<Street> hStreets;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	inline glm::vec3 center() const;
	void makeStreets();
	void makeAreas(const std::vector<Street>& v, const std::vector<Street>& h);
	void fillAreas();
	void fillArea(const Area&);
	int calculateHeightForArea(const Area& area) const;
	float calculateHeightCoefficientForDistance(float dist) const;
};

glm::vec3 World::center() const
{
	return glm::vec3{ worldWidth / 2.0f, 0.0f, -worldHeight / 2.0f };
}

