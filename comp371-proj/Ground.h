#pragma once
#include "Street.h"

/**
*	@class handles the creation and rendering of the city streets and the ground
*	@author Hrachya Hakobyan
*/
class Ground
{
public:
	/**
	*	Create a ground model for the world.
	*	@param bottomleft the bottomleft corner of the world
	*	@param width the width of the world in X direction
	*	@param height the width of the world in the negative Z direction
	*/
	static std::shared_ptr<glutil::Model> makeGround(const glm::vec3& bottomleft, int width, int height);
	/**
	*	Construct streets.
	*	@param bottomleft the bottomleft corner of the world
	*	@param worldWidth the width of the world in X direction
	*	@param worldHeight the width of the world in negative Z direction
	*	@param v the vertices vector to store into
	*	@param i the indices vector to store into
	*	@param vertical the vector of Street objects to store the vertical streets (parallel to Z axis, in XZ plane)
	*	@param horizontal the vector of Street objects to store the horizontal streets (parallel to X axis, in XZ plane)
	*/
	static void makeStreets(const glm::vec3& bottomleft, int worldWidth, int worldHeight,
							std::vector<glutil::Vertex>& v, std::vector<GLuint>& i,
							std::vector<Street>& vertical,
							std::vector<Street>& horizontal);
	/**
	*	Construct the street mesh given the street data
	*	@param s the street data
	*	@param vertices the vector of vertices to store into
	*	@param indices the vector of indices to store into
	*/
	static void makeStreet(const Street& s, std::vector<glutil::Vertex>& vertices,
							std::vector<GLuint>& indices);
};

