#pragma once
#include "World.h"

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
	glm::mat4 view;
	glm::mat4 proj;
	World world;
};

