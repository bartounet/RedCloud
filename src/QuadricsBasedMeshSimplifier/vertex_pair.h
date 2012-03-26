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

#ifndef VERTEX_PAIR_H_
#define VERTEX_PAIR_H_


#include "vertex.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class VertexPair
{

public:
	VertexPair(Vertex* parV0, Vertex* parV1);
	~VertexPair();

public:
	uint Contract(std::vector<VertexPair*>& parDeletePairs, std::vector<VertexPair*>& parUpdatePairs);

public:
	void ComputePosAndQuadric();
	void RemoveOnRelatedVertex();

public:
	inline bool IsDegenerated() const { return (v0_ == v1_); }
	inline bool DeleteMe() const { return deleteMe_; }
	inline void SetDeleteMe() { assert(!deleteMe_); deleteMe_ = true; }
	inline Vertex* V0() { return v0_; }
	inline Vertex* V1() { return v1_; }
	inline uint HeapInd() const { return heapInd_; }
	inline void SetHeapInd(uint parInd) { heapInd_ = parInd; }
	void SetVertices(Vertex* parV0, Vertex* parV1);

#ifdef _DEBUG
	double QuadricError() const;
	void AssignQuadricErrorWithNewValue();
	void UnsetQuadricErrorComputed();
#else
	inline double QuadricError() const { return quadricError_; }
	inline void AssignQuadricErrorWithNewValue() { quadricError_ = newQuadricError_; }
#endif

private:
	void ComputeOptimalPos_();
	double ComputeQuadricError_(const Com::Vec4& parPos) const;

private:
	Vertex* v0_;
	Vertex* v1_;
	Com::Vec4 pos_;
	Quadric quadric_;

	double quadricError_;
	double newQuadricError_;
	uint heapInd_;

	bool deleteMe_;

#ifdef _DEBUG
	bool quadricErrorComputed_;
#endif
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 