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

#ifndef FACE_H_
#define FACE_H_


#include "../common/geometry.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Vertex;
class Face
{
public:
	Face();
	Face(Vertex* parV0, Vertex* parV1, Vertex* parV2);
	~Face();

public:
	bool IsDegenerated() const;
	bool HasZeroAreaSurface() const;

public:
	void RemoveOnRelatedVertex();

public:
	Vertex* V0() const { return v0_; }
	Vertex* V1() const { return v1_; }
	Vertex* V2() const { return v2_; }
	bool DeleteMe() const { return deleteMe_; }
	void SetV0(Vertex* parVertex) { assert(parVertex); v0_ = parVertex; }
	void SetV1(Vertex* parVertex) { assert(parVertex); v1_ = parVertex; }
	void SetV2(Vertex* parVertex) { assert(parVertex); v2_ = parVertex; }
	void SetDeleteMe() { deleteMe_ = true; }

private:
	Vertex* v0_;
	Vertex* v1_;
	Vertex* v2_;
	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 