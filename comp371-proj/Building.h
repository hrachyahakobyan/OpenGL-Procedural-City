#pragma once

enum BuildingType{
	Classical = 0, Blocky, Tower, Cube
};

class Building
{
public:
	static std::shared_ptr<glutil::Model> make(BuildingType, const glm::vec3& topleft, int xWidth, int zWidth, int height);
private:
	static std::shared_ptr<glutil::Model> makeClassical(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	static std::shared_ptr<glutil::Model> makeBlocky(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	static std::shared_ptr<glutil::Model> makeTower(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	static std::shared_ptr<glutil::Model> makeCube(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	static void constructRoof(glutil::Model& building, BuildingType type, const glm::vec3& topleft,  int xWidth, int zWidth);
	static void triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height);
};

