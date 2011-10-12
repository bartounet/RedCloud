#ifndef TYPES_H_
#define TYPES_H_


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

	// FIXME: Cross product, Dot product and Normalize
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif