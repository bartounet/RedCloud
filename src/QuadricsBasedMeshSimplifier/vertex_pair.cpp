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

#include "vertex_pair.h"
#include "quadric.h"
#include <assert.h>
#include <limits>
#include <algorithm>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
VertexPair::VertexPair(Vertex* parV0, Vertex* parV1) :
	v0_(parV0),
	v1_(parV1),
	quadricError_(0.0),
	newQuadricError_(-1.0),
	deleteMe_(false)
#ifdef _DEBUG
	, quadricErrorComputed_(false)
#endif
{
	assert(v0_);
	assert(v1_);
	assert(v0_ < v1_);
}
// ----------------------------------------------------------------------------
VertexPair::~VertexPair()
{
}
// ----------------------------------------------------------------------------
void VertexPair::ComputeOptimalPos_()
{
#ifdef _DEBUG
	assert(!quadricErrorComputed_);
#endif

// FIXME: Rendre ce code plus classe et gerer le point optimal !

	const Com::Vec4& pos0 = v0_->Pos();
	const Com::Vec4& pos1 = v1_->Pos();

	pos_ = pos0;
	double minError = ComputeQuadricError_(pos0);

	Com::Vec4 midPos;
	midPos.x = (pos0.x + pos1.x) / 2;
	midPos.y = (pos0.y + pos1.y) / 2;
	midPos.z = (pos0.z + pos1.z) / 2;
	double error = ComputeQuadricError_(midPos);
	if (error < minError)
	{
		pos_ = midPos;
		minError = error;
	}

	error = ComputeQuadricError_(pos1);
	if (error < minError)
	{
		pos_ = pos1;
		minError = error;
	}

	newQuadricError_ = minError;
#ifdef _DEBUG
	quadricErrorComputed_ = true;
#endif
}
// ----------------------------------------------------------------------------
#ifdef _DEBUG // Non-debug version is inlined
double VertexPair::QuadricError() const
{
	assert(quadricErrorComputed_ || deleteMe_);
	return quadricError_;
}
#endif
// ----------------------------------------------------------------------------
#ifdef _DEBUG // Non-debug version is inlined
void VertexPair::UnsetQuadricErrorComputed()
{
	assert(quadricErrorComputed_);
	quadricErrorComputed_ = false;
}
#endif
// ----------------------------------------------------------------------------
#ifdef _DEBUG // Non-debug version is inlined
void VertexPair::AssignQuadricErrorWithNewValue()
{
	assert(quadricErrorComputed_);
	quadricError_ = newQuadricError_;
}
#endif
// ----------------------------------------------------------------------------
void VertexPair::ComputePosAndQuadric()
{
#ifdef _DEBUG
	assert(!quadricErrorComputed_);
#endif

	quadric_.Init();
	quadric_.Add(v0_->GetQuadric());
	quadric_.Add(v1_->GetQuadric());

	ComputeOptimalPos_();
}
// ----------------------------------------------------------------------------
double VertexPair::ComputeQuadricError_(const Com::Vec4& parPos) const
{
#ifdef _DEBUG
	assert(!quadricErrorComputed_);
#endif

	double x = parPos.x;
	double y = parPos.y;
	double z = parPos.z;
	double w = parPos.w;
	const double* q = quadric_.Values();

	double error = 0.0;
	error += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	error += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	error += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	error += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;
	if (error < 0.0) // floating round error hack
		error = 0.0;

	return error;
}
// ----------------------------------------------------------------------------
uint VertexPair::Contract(std::vector<VertexPair*>& parDeletePairs, std::vector<VertexPair*>& parUpdatePairs)
{
	assert(!deleteMe_);
	assert(!IsDegenerated());
	assert(parDeletePairs.size() == 0);
	assert(parUpdatePairs.size() == 0);
#ifdef _DEBUG
	assert(quadricErrorComputed_);
#endif

	uint nbFacesDeleted = 0;

	v0_->SetPos(pos_);
	v0_->SetQuadric(quadric_);

	v1_->ReplaceThisInIncidentFacesWith(v0_);
	nbFacesDeleted += v1_->RemoveDegeneratedFaces();
	v0_->AddIncidentFaces(v1_->IncidentFaces());
	nbFacesDeleted += v0_->RemoveDegeneratedFaces();

	deleteMe_ = true;
	v1_->RemovePair(this);
	v0_->RemovePair(this);

	v1_->ReplaceThisInPairsWith(v0_); // warning: call after remove THIS pair
	assert(v0_ != v1_);
	v1_->RemoveDegeneratedPairs(parDeletePairs);
	v0_->AddPairs(v1_->Pairs());
	v0_->RemoveDegeneratedPairs(parDeletePairs); // FIXME: des pairs en doublon du coup (parDeletePairs) ?
	v0_->RemoveDuplicatedPair(parDeletePairs);

	v1_->SetDeleteMe();

	v0_->UpdatePairPosAndQuadric(parUpdatePairs);

	return nbFacesDeleted;
}
// ----------------------------------------------------------------------------
void VertexPair::SetVertices(Vertex* parV0, Vertex* parV1)
{
	assert(parV0);
	assert(parV1);

	if (parV0 < parV1)
	{
		v0_ = parV0;
		v1_ = parV1;
	}
	else
	{
		v0_ = parV1;
		v1_ = parV0;
	}

#ifdef _DEBUG
	quadricErrorComputed_ = false;
#endif
}
// ----------------------------------------------------------------------------
void VertexPair::RemoveOnRelatedVertex()
{
	assert(deleteMe_);
	assert(!IsDegenerated());

	v0_->RemovePair(this);
	v1_->RemovePair(this);
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}