#pragma once
class Textures
{
public:
	static std::shared_ptr<glutil::Texture> randomWindow();
	static std::shared_ptr<glutil::Texture> randomFacade();
	static std::shared_ptr<glutil::Texture> randomFloor();
private:
	static const int windows = 7;
	static const int facades = 5;
	static const int floors = 1;
};

