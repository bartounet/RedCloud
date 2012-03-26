//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TYPES_H_
#define TYPES_H_


#include <math.h>
#include <assert.h>


typedef unsigned int uint;
typedef unsigned char uchar;


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace Com
{
// ----------------------------------------------------------------------------

struct Vec4
{
	double x;
	double y;
	double z;
	double w;

	Vec4() :
		x(0.0), y(0.0), z(0.0), w(1.0)
	{
	}

	Vec4(double parX, double parY, double parZ, double parW) :
		x(parX), y(parY), z(parZ), w(parW)
	{
	}

	Vec4(const Vec4& parFirst, const Vec4& parSecond) :
		x(parSecond.x - parFirst.x),
		y(parSecond.y - parFirst.y),
		z(parSecond.z - parFirst.z),
		w(1.0)
	{
	}

	Vec4& operator=(const Vec4& parRhs)
	{
		x = parRhs.x;
		y = parRhs.y;
		z = parRhs.z;
		w = parRhs.w;

		return *this;
	}

	double Length() const
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	static Vec4 CrossProduct(const Vec4& parFirst, const Vec4& parSecond)
	{
		double x = parFirst.y * parSecond.z - parFirst.z * parSecond.y;
		double y = parFirst.z * parSecond.x - parFirst.x * parSecond.z;
		double z = parFirst.x * parSecond.y - parFirst.y * parSecond.x;
		return Vec4(x, y, z, 1.0);
	}

	static Vec4 Normalize(const Vec4& parVec4)
	{
		double length = parVec4.Length();
		assert(length > 0.0f);
		return Vec4(parVec4.x / length, parVec4.y / length, parVec4.z / length, 1.0);
	}
};
// ----------------------------------------------------------------------------
struct Vec2
{
	double x;
	double y;

	Vec2() :
		x(0.0), y(0.0)
	{
	}

	Vec2(double parX, double parY) :
		x(parX), y(parY)
	{
	}

	Vec2(const Vec2& parFirst, const Vec2& parSecond) :
		x(parSecond.x - parFirst.x),
		y(parSecond.y - parFirst.y)
	{
	}

	Vec2& operator=(const Vec4& parRhs)
	{
		x = parRhs.x;
		y = parRhs.y;

		return *this;
	}

	double Length() const
	{
		return sqrt((x * x) + (y * y));
	}

	static Vec2 Normalize(const Vec4& parVec4)
	{
		double length = parVec4.Length();
		assert(length > 0.0f);
		return Vec2(parVec4.x / length, parVec4.y / length);
	}
};
// ----------------------------------------------------------------------------
struct Color
{
    uchar r;
    uchar g;
    uchar b;

    Color (uchar parR, uchar parG, uchar parB) :
		r(parR),
		g(parG),
		b(parB)
	{
	}
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif