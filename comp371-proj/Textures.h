#pragma once
/**
*	@brief class handles loading of different textures used in the project
*	@author Hrachya Hakobyamn
*/
class Textures
{
public:
	static std::shared_ptr<glutil::Texture> randomWindow();
	static std::shared_ptr<glutil::Texture> randomFacade();
	static std::shared_ptr<glutil::Texture> randomRoof();
	static std::shared_ptr<glutil::Texture> randomRoofTile();
	static std::shared_ptr<glutil::Texture> randomDarkBrick();
	static std::shared_ptr<glutil::Texture> randomLightBrick();
	static std::shared_ptr<glutil::Texture> randomSidewalk();
	static std::shared_ptr<glutil::Texture> randomRoad();
private:
	static const int windows = 17;
	static const int facades = 5;
	static const int roofs = 8;
	static const int roofTiles = 7;
	static const int roads = 2;
	static const int sidewalks = 9;
	static const int bricks = 4;
	static const int bricks_dark = 3;
};

