#include "stdafx.h"
#include "Textures.h"
#include "Random.h"

std::shared_ptr<glutil::Texture> Textures::randomWindow()
{
	int w = Random::random(1, windows);
	std::string name = "textures\\window" + std::to_string(w) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomFacade()
{
	int f = Random::random(1, facades);
	std::string name = "textures\\facade" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomFloor()
{
	int f = Random::random(1, floors);
	std::string name = "textures\\floor" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}