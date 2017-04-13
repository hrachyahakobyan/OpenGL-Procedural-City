#pragma once
#include "Area.h"

enum BuildingType{
	Classical = 0, Blocky, Tower, Cube
};

class World;

class Building
{
public:
	Building(const World& worldRef, const std::vector<Area>& areas, std::size_t partitions);
	void update();
	void draw();
private:
	typedef std::shared_ptr<glutil::Model> ModelPtr;
	typedef std::tuple<std::vector<glutil::Vertex>, std::vector<GLuint>, std::shared_ptr<glutil::Texture>> VertexIndexTuple;
	typedef std::map<GLuint, VertexIndexTuple> VertexIndexMap;
	std::vector<ModelPtr> buildings;
	std::vector<std::vector<ModelPtr>> buildingGrids;

	const World& world;
	const int maxHeight = 50;

	void fillArea(const Area& area, VertexIndexMap& map);
	void makeBuilding(const Area& area, int height, VertexIndexMap& map);
	void makeCube(const Area& area, int height, VertexIndexMap& map);
	void makeBlocky(const Area& area, int height, VertexIndexMap& map);
	void makeRoof(const Area& area, BuildingType type, VertexIndexMap& map);
	void makeClassical(const Area& area, int height, VertexIndexMap& map);
	void makeTower(const Area& area, int height, VertexIndexMap& map);

	int calculateHeightForArea(const Area& area) const;
	float calculateHeightCoefficientForDistance(float dist) const;
	ModelPtr make(BuildingType, const glm::vec3& topleft, int xWidth, int zWidth, int height);
	ModelPtr makeClassical(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	ModelPtr makeBlocky(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	ModelPtr makeTower(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	ModelPtr makeCube(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	void constructRoof(glutil::Model& building, BuildingType type, const glm::vec3& topleft,  int xWidth, int zWidth);
	void triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height);
};

