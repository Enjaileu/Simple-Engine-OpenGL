#include "Vector3.h"

const Vector3 Vector3::zero(0.0f, 0.0f, 0.f);
const Vector3 Vector3::unitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::unitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::unitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::negUnitX(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::negUnitY(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::negUnitZ(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::infinity(Maths::infinity, Maths::infinity, Maths::infinity);
const Vector3 Vector3::negInfinity(Maths::negInfinity, Maths::negInfinity, Maths::negInfinity);

void Vector3::Set(float xP, float yP, float zP)
{
	x = xP;
	y = yP;
	z = zP;
}

float Vector3::LengthSq() const
{
	return (x * x + y * y + z * z);
}

float Vector3::Length() const
{
	return (Maths::sqrt(LengthSq()));
}

void Vector3::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
}