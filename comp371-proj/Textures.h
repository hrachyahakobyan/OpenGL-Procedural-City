#pragma once
class Textures
{
public:
	static std::shared_ptr<glutil::Texture> randomWindow();
	static std::shared_ptr<glutil::Texture> randomFacade();
	static std::shared_ptr<glutil::Texture> randomRoof();
	static std::shared_ptr<glutil::Texture> randomGround();
private:
	static const int windows = 17;
	static const int facades = 5;
	static const int grounds = 1;
	static const int roofs = 1;
};

