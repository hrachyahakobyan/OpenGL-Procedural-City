#pragma once
#include "Area.h"
#include "Ground.h"
#include "Vehicles.h"
#include "Sidewalks.h"
#include "Building.h"

/**
*	@brief class connecting different parts of the world together, orchestrating the rendering and update
*	@author Hrachya Hakobyan
*/
class World
{
public:
	World();
	~World();
	/**
	*	Initialize the world
	*	@param worldWidth the width in X direction
	*	@param worldHeight the width in negative Z direction
	*/
	void initialize(int worldWidth, int worldHeight);
	/**
	*	Update the world parameters
	*	@param view the view matrix
	*	@param proj the projection matrix
	*	@param viewPos the position of the viewer, i.e. the camera
	*/
	void update(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);
	void draw();
	inline glm::vec3 getBackgroundColor() const { return lightColor * fogColor; }
	inline const glm::mat4& getView() const { return view; }
	inline const glm::mat4& getProj() const { return proj; }
	inline const glm::vec3& getViewPos() const { return viewPos; }
	inline int getWorldHeight() const { return worldHeight; }
	inline int getWorldWidth() const { return worldWidth; }
	inline const glm::vec3& getLightPos() const { return lightPos; }
	inline const glm::vec3& getLightColor() const { return lightColor; }
	inline const glm::vec3& getLightDirection() const { return lightDirection; }
	/**
	*	@return the primary shader of the world
	*/
	inline const glutil::Shader& getShader() const { return *bldShader; }
	/**
	*	@return the time elapsed since last render
	*/
	inline const GLfloat getDeltaTime() const { return deltaTime; }
	/**
	*	@return the center of the world
	*/
	inline glm::vec3 center() const;
	/**
	*	Collision detection with a 3D point.
	*	@param a 3D point to query against
	*	@return true if any entitiy in the world contains the specified point
	*/
	bool intersectsPoint(const glm::vec3& point) const;
	/**
	*	Return the grid coordinate of the area for the specified sizes of the grid
	*	@param a the area 
	*	@param width the width of the grid
	*	@param height the height of the grid
	*	@return a pair of coordinates in Z and X directions specifying the row and the column of the coordinate 
	*	The origin of the grid is considered the topleft corner of the world
	*/
	std::pair<std::size_t, std::size_t> areaCoordinate(const Area& a, std::size_t width, std::size_t height) const;
private:
	std::size_t gridPartitions;
	typedef std::shared_ptr<glutil::Model> ModelPtr;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 viewPos;
	int maxHeight;
	int worldWidth;
	int worldHeight;
	glm::vec3 lightPos;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 fogColor;
	std::unique_ptr<Vehicles> vehicles;
	std::unique_ptr<Sidewalks> sidewalks;
	std::unique_ptr<Building> buildings;
	std::shared_ptr<glutil::Shader> bldShader;
	std::shared_ptr<glutil::Model> ground;
	std::shared_ptr<glutil::Model> streets;
	std::shared_ptr<glutil::Skybox> skybox;
	std::vector<Area> areas;
	std::vector<std::vector<std::vector<const Area*>>> areaGrid;
	std::vector<Street> vStreets;
	std::vector<Street> hStreets;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	void makeStreets();
	void makeAreas(const std::vector<Street>& v, const std::vector<Street>& h);
	void initializeAreaGrid();
	std::pair<std::size_t, std::size_t> gridCoordinate(const glm::vec3& vec, bool& contains) const;
};

glm::vec3 World::center() const
{
	return glm::vec3{ worldWidth / 2.0f, 0.0f, -worldHeight / 2.0f };
}

