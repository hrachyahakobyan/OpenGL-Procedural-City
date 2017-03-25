#pragma once

struct Area{
	glm::vec3 bottomleft;
	int xWidth;
	int zWidth;
	Area(const glm::vec3& bottomleft, int xWidth, int zWidth): bottomleft(bottomleft), xWidth(xWidth), zWidth(zWidth){}
};

enum ObjectType{
	Road = 0, Building
};

class TestApp : public glutil::ApplicationBase
{
public:
	explicit TestApp(const std::string& windowName);
	explicit TestApp(std::unique_ptr<glutil::WindowHandler> windowHandler);
	TestApp() = delete;
	TestApp(const TestApp&) = delete;
	TestApp& operator=(const TestApp& other) = delete;
	~TestApp();
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	void makeStreets();
	std::shared_ptr<glutil::Shader> shader;
	std::shared_ptr<glutil::Model> ground;
	std::vector<std::shared_ptr<glutil::Model>> streets;
	std::vector<std::shared_ptr<glutil::Model>> buildings;
	std::shared_ptr<glutil::Skybox> skybox;
	std::vector<Area> areas;
	std::vector<std::vector<ObjectType>> map;
	glm::mat4 view;
	glm::mat4 proj;
};

