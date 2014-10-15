/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	coord.h

	좌표계 관련 정의
=======================================================*/

#pragma once

#include "../internal.h"
#include <math.h>

typedef Vector2f vector_t;
typedef Vector2f pos_t;

#define PI       3.14159265358979323846f
#define PI_2     1.57079632679489661923f
#define PI_4     0.785398163397448309616f

inline IntRect operator + (const Vector2i & lhs, const IntRect & rhs)
{
	return IntRect(	lhs.x + rhs.left,
					lhs.y + rhs.top,
					rhs.width ,
					rhs.height);
}

template <typename T>
inline float operator * (const T & lhs, const T & rhs)
{
	return static_cast<float>(lhs.x * rhs.x + lhs.y * rhs.y);
}

template <typename T>
inline float GetDistance(const T & v1, const T & v2)
{
	T r = v1 - v2;
	return sqrtf(r * r);
}

template <typename T>
inline float GetSize(const T & v)
{
	return sqrtf(v * v);
}

template <typename T>
inline T GetUnitVector(float dir)
{
	return T(sinf(dir), -cosf(dir));
}

template <typename T>
inline T GetUnitVector(const T & v)
{
	float size = GetSize(v);
	return v / size;
}

inline float GetDir(const vector_t & v)
{
	float angle = atan2(v.x, -v.y) * 180.f / PI;
	if(angle < 0.f) return angle + 360.f;
	else return angle;
}

inline float ToDegree(float angle)
{
	return 180.f * angle / static_cast<float>(PI);
}
