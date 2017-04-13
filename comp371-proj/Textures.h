#pragma once
class Textures
{
public:
	static std::shared_ptr<glutil::Texture> randomWindow();
	static std::shared_ptr<glutil::Texture> randomFacade();
	static std::shared_ptr<glutil::Texture> randomRoof();
	static std::shared_ptr<glutil::Texture> randomGround();
	static std::shared_ptr<glutil::Texture> randomSidewalk();
	static std::shared_ptr<glutil::Texture> randomRoad();
private:
	static const int windows = 17;
	static const int facades = 5;
	static const int grounds = 4;
	static const int roofs = 6;
	static const int roads = 2;
	static const int sidewalks = 9;
};

