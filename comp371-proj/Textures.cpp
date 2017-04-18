#include "stdafx.h"
#include "Textures.h"
#include "Random.h"

std::shared_ptr<glutil::Texture> Textures::randomWindow()
{
	int w = Random::random(1, windows);
	std::string name = "textures\\window" + std::to_string(w) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomRoad()
{
	int w = Random::random(1, roads);
	std::string name = "textures\\road" + std::to_string(w) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomFacade()
{
	int f = Random::random(1, facades + bricks + bricks_dark);
	std::string textName;
	if (f <= facades)
		textName = "facade";
	else if (f <= bricks + facades) {
		textName = "brick";
		f -= facades;
	}
	else {
		textName = "brick_dark";
		f -= facades + bricks;
	}
	std::string name = "textures\\" + textName + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomDarkBrick()
{
	int f = Random::random(1, bricks_dark);
	std::string name = "textures\\brick_dark" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomLightBrick()
{
	int f = Random::random(1, bricks);
	std::string name = "textures\\brick" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomRoof()
{
	int f = Random::random(1, roofs);
	std::string name = "textures\\roof" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomRoofTile()
{
	int f = Random::random(1, roofTiles);
	std::string name = "textures\\rooftiles" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}

std::shared_ptr<glutil::Texture> Textures::randomSidewalk()
{
	int f = Random::random(1, sidewalks);
	std::string name = "textures\\sidewalk" + std::to_string(f) + ".jpg";
	return glutil::Texture::fromFile(name, GL_TEXTURE_2D, glutil::Diffusive);
}