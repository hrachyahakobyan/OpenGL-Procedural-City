#pragma once

enum BuildingType{
	Classical = 0, Blocky
};

class Building
{
public:
	static std::shared_ptr<glutil::Model> make(BuildingType, const glm::vec3& topleft, int xWidth, int zWidth, int height);
private:
	static std::shared_ptr<glutil::Model> makeClassical(const glm::vec3& topleft, int xWidth, int zWidth, int height);
	static std::shared_ptr<glutil::Model> makeBlocky(const glm::vec3& topleft, int xWidth, int zWidth, int height);

	static void constructRoof(glutil::Model& building, BuildingType type, const glm::vec3& topleft, int height, int xWidth, int zWidth);
};

