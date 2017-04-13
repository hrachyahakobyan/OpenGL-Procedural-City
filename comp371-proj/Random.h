#pragma once
#include <random>

class Random
{
public:
	static int random(int min, int max);
	static int normal(int mean, int stdev, bool clamp = true);
	static bool dice(float probability);
	static bool coin();
private:
	static std::default_random_engine generator;
};

