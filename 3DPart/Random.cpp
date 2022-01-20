#include "Random.h"

void Random::Init() {
	std::random_device rd;
	Random::Seed(rd());
}

void Random::Seed(unsigned int seed) {
	sGenerator.seed(seed);
}

float Random::GetFloat() {
	return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float min, float max) {
	std::uniform_int_distribution<int>dist(min, max);
	return dist(sGenerator);
}

Vector2 Random::GetVector(const Vector2& min, const Vector2& max) {
	Vector2 r = Vector2(GetFloat(), GetFloat());
	Vector2 diff = max - min;
	return min + Vector2(diff.x * r.x, diff.y * r.y);
}

std::mt19937 Random::sGenerator;
