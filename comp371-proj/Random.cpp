#include "stdafx.h"
#include "Random.h"

std::default_random_engine Random::generator;

int Random::random(int min, int max)
{
	static std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
	return min + dis(generator) % (max - min + 1);
}

int Random::normal(int mean, int stdev, bool clamp)
{
	std::normal_distribution<double> distribution(mean, stdev);
	return static_cast<int>(distribution(generator));
}