#pragma once
#include <random>

/**
*	@class Handles random number generation
*/
class Random
{
public:
	/**
	*	@return uniform random integer in the range inclusive
	*	@param min the lower bound
	*	@param max the upper bound
	*/
	static int random(int min, int max);
	/**
	*	@return integer from normal distribution
	*	@param mean the mean of distribution
	*	@param stdev the standard distribution
	*	@param clamp the values to the range [mean-stdev, mean+stdev]
	*/
	static int normal(int mean, int stdev, bool clamp = true);
	/**
	*	@param probability the chance to return true, in range [0.0f, 1.0f]
	*	@return true with a given probability
	*/
	static bool dice(float probability);
	/**
	*	@return true with 50% probability
	*/
	static bool coin();
private:
	static std::default_random_engine generator;
};

