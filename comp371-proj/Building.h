#pragma once
#include "Area.h"

enum BuildingType{
	Classical = 0, Blocky, Tower, Cube
};

class World;

/**
*	@brief class to handle building construction and rendering
*	@author Hrachya Hakobyan
*/
class Building
{
public:
	/**
	*	@param worldRef a reference to the world the building belongs to
	*	@param areas a vector of areas to build buildings on
	*	@partitions the sqrt of number of grid partitions to divide the buildings into
	*/
	Building(const World& worldRef, std::vector<Area>& areas, std::size_t partitions);
	void update();
	void draw();
private:
	typedef std::shared_ptr<glutil::Model> ModelPtr;
	typedef std::tuple<std::vector<glutil::Vertex>, std::vector<GLuint>, std::shared_ptr<glutil::Texture>> VertexIndexTuple;
	typedef std::map<GLuint, VertexIndexTuple> VertexIndexMap;
	std::vector<ModelPtr> buildings;
	std::vector<std::vector<ModelPtr>> buildingGrids;

	const World& world;
	const int maxHeight = 70;

	/**
	*	Fill the area with a building. Based on the type of the building built,
	*	the area will receive Volume objects
	*	@param area to add buildings to
	*	@param map the vertex index map for the grid unit the area belongs to
	*/
	void fillArea(Area& area, VertexIndexMap& map);
	/**
	*	Construct a building on the specified area
	*	@param area the area to build a building on
	*	@param height the estimated height of the building
	*	@param map the vertex index map for the current grid unit
	*/
	void makeBuilding(Area& area, int height, VertexIndexMap& map);

	void makeCube(Area& area, int height, VertexIndexMap& map);
	void makeBlocky(Area& area, int height, VertexIndexMap& map);
	void makeRoof(const Area& area, BuildingType type, VertexIndexMap& map, Area& bldArea);
	void makeClassical(Area& area, int height, VertexIndexMap& map);
	void makeTower(Area& area, int height, VertexIndexMap& map);

	/**
	*	Calculates the estimated height for a building in the specified area.
	*	The height is calculated based on the distance of the center of the area from the
	*	center of the world. The distance is inversly (but not necessarily linearly) 
	*	proportional to the distance from the center. The height contains
	*	error terms from Gaussian distribution.
	*	@param area the area of the building
	*/
	int calculateHeightForArea(const Area& area) const;
	float calculateHeightCoefficientForDistance(float dist) const;
	/**
	*	Triangulates the given vector of points. Uses only the X and Z coordinates of the points.
	*	The y coordinate is specified by the height parameter. Assumes the points are in CW direction.
	*	Triangulates by calculating the centroid of the points and connection each two consecutive
	*	points with the centroid. If the points are in CW direction, it is guaranteed that the vertices
	*	will have correct normals directed in the (0,1,0) direction.
	*	@param points the vector of points
	*	@param vertices the vector of vertices to insert into
	*	@param indices the vector of indices to insert into
	*	@param height the y parameter of vertices.
	*	@param centroidHeightOffset the height offset of the centroid. The y coordinate of the centroid will calculated by
	*	height + centroidHeightOffset
	*/
	void triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height, float centroidHeightOffset = 0.0f);
};

