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

#include "face.h"
#include "vertex.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
Face::Face() :
	v0_(0),
	v1_(0),
	v2_(0),
	deleteMe_(false)
{
}
// ----------------------------------------------------------------------------
Face::Face(Vertex* parV0, Vertex* parV1, Vertex* parV2) :
	v0_(parV0),
	v1_(parV1),
	v2_(parV2),
	deleteMe_(false)
{
	assert(v0_);
	assert(v1_);
	assert(v2_);
	assert(!IsDegenerated());
}
// ----------------------------------------------------------------------------
Face::~Face()
{
}
// ----------------------------------------------------------------------------
void Face::RemoveOnRelatedVertex()
{
	assert(IsDegenerated());

	if (v0_ == v1_)
	{
		if (v0_ != v2_)
			v2_->RemoveIncidentFace(this);
	}
	else
		v1_->RemoveIncidentFace(this);

	v0_->RemoveIncidentFace(this);

	deleteMe_ = true;
}
// ----------------------------------------------------------------------------
bool Face::IsDegenerated() const
{
	return ((v0_ == v1_) || (v0_ == v2_) || (v1_ == v2_)); // less than 3 vertices
}
// ----------------------------------------------------------------------------
bool Face::HasZeroAreaSurface() const
{
	Com::Vec4 edge1(v0_->Pos(), v1_->Pos());
	Com::Vec4 edge2(v0_->Pos(), v2_->Pos());
	Com::Vec4 normal = Com::Vec4::CrossProduct(edge1, edge2);

	return (normal.Length() <= 0.0);
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
