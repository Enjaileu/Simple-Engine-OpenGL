#pragma once
#include <random>
#include "Vector2.h"

class Random
{
public:
	static void Init();
	static void Seed(unsigned int seed);

	//get random float between 0 and 1
	static float GetFloat();
	//get random float in a range
	static float GetFloatRange(float min, float max);
	//get random int in range
	static int GetIntRange(int min, int max);
	//get random vector with min/max bounds
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
private:
	static std::mt19937 sGenerator;
};

