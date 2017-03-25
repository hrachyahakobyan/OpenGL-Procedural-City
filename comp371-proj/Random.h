#pragma once
#include <random>

class Random
{
public:
	static int random(int min, int max);
	static int normal(int mean, int stdev, bool clamp = true);
private:
	static std::default_random_engine generator;
};

