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

std::shared_ptr<glutil::Texture> Textures::randomGround()
{
	int f = Random::random(1, grounds);
	std::string name = "textures\\ground" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}


std::shared_ptr<glutil::Texture> Textures::randomRoof()
{
	int f = Random::random(1, roofs);
	std::string name = "textures\\roofs" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}