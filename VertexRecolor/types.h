#ifndef TYPES_H_
#define TYPES_H_


#include <math.h>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace VR
{
// ----------------------------------------------------------------------------
typedef unsigned int uint;
typedef unsigned char uchar;

struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	Vec4() :
		x(0.f), y(0.f), z(0.f), w(1.f)
	{
	}

	Vec4(float parX, float parY, float parZ, float parW) :
		x(parX), y(parY), z(parZ), w(parW)
	{
	}

	Vec4(const Vec4& parFirst, const Vec4& parSecond) :
		x(parSecond.x - parFirst.x),
		y(parSecond.y - parFirst.y),
		z(parSecond.z - parFirst.z),
		w(1.f) // ok ?
	{
	}

	static Vec4 CrossProduct(const Vec4& parFirst, const Vec4& parSecond)
	{
		float x = parFirst.y * parSecond.z - parFirst.z * parSecond.y;
		float y = parFirst.z * parSecond.x - parFirst.x * parSecond.z;
		float z = parFirst.x * parSecond.y - parFirst.y * parSecond.x;
		return Vec4(x, y, z, 1.f);
	}

	static Vec4 Normalize(const Vec4& parVec4)
	{
		float length = sqrt((parVec4.x * parVec4.x) + (parVec4.y * parVec4.y) + (parVec4.z * parVec4.z));
		assert(length > 0.f);
		return Vec4(parVec4.x / length, parVec4.y / length, parVec4.z / length, 1.f);
	}

	// FIXME: Dot product
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif