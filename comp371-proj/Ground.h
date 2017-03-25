#pragma once

enum StreetDirection{
	Vertical, Horizontal
};

class Ground
{
public:
	static std::shared_ptr<glutil::Model> make(const glm::vec3& bottomleft, int width, int height);
	static std::shared_ptr<glutil::Model> makeStreet(const glm::vec3& bottomleft, StreetDirection dir, int width, int height);
};

