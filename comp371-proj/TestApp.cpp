#include "stdafx.h"
#include "TestApp.h"


TestApp::TestApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

TestApp::TestApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

TestApp::~TestApp()
{
	glutil::Texture::purge();
}

void TestApp::initialize()
{
	glutil::ApplicationBase::initialize();
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	camera.reset(new glutil::Camera(glm::vec3(0.0f, 50.f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)));
	camera->setMovementSpeed(10.f);
	world.initialize(128, 128);
}


void TestApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	view = camera->view();
	// Projection 
	proj = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 100.0f);
	// Get the uniform locations
	world.update(view, proj, camera->getPosition());
}

void TestApp::render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	world.draw();
}

void TestApp::updateMovement()
{
	if (keys[GLFW_KEY_W])
		camera->move(glutil::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->move(glutil::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->move(glutil::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->move(glutil::RIGHT, deltaTime);
}

