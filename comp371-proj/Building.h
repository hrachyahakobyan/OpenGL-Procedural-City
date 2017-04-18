#pragma once
#include "Area.h"

enum BuildingType{
	Classical = 0, Blocky, Tower, Cube
};

class World;

class Building
{
public:
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

	void fillArea(Area& area, VertexIndexMap& map);
	void makeBuilding(Area& area, int height, VertexIndexMap& map);
	void makeCube(Area& area, int height, VertexIndexMap& map);
	void makeBlocky(Area& area, int height, VertexIndexMap& map);
	void makeRoof(const Area& area, BuildingType type, VertexIndexMap& map, Area& bldArea);
	void makeClassical(Area& area, int height, VertexIndexMap& map);
	void makeTower(Area& area, int height, VertexIndexMap& map);

	int calculateHeightForArea(const Area& area) const;
	float calculateHeightCoefficientForDistance(float dist) const;
	void triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height, float centroidHeightOffset = 0.0f);
};

