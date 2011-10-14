
#include "vertex.h"
#include "../VertexRecolor/types.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
Vertex::Vertex(float parX, float parY, float parZ) :
	pos_(parX, parY, parZ, 1.f),
	associatedQuadric_(0),
	quadricErrorComputed_(false),
	quadricError_(0.f)
{
}
// ----------------------------------------------------------------------------
Vertex::Vertex(const VR::Vertex& parVertex) :
	pos_(parVertex.x, parVertex.y, parVertex.z, 1.f),
	associatedQuadric_(0),
	quadricErrorComputed_(false),
	quadricError_(0.f)
{
}
// ----------------------------------------------------------------------------
Vertex::Vertex() :
	pos_(0.f, 0.f, 0.f, 1.f),
	associatedQuadric_(0),
	quadricErrorComputed_(false),
	quadricError_(0.f)
{
}
// ----------------------------------------------------------------------------
void Vertex::AddIncidentFace(const Face* parFace)
{
	incidentFaces_.push_back(parFace);
}
// ----------------------------------------------------------------------------
Vertex::~Vertex()
{
	if (associatedQuadric_)
		delete associatedQuadric_;
}
// ----------------------------------------------------------------------------
void Vertex::SetAssociatedQuadric(Quadric* parQuadric)
{
	assert(parQuadric);
	associatedQuadric_ = parQuadric;
	quadricErrorComputed_ = false;
	QuadricError();
}
// ----------------------------------------------------------------------------
void Vertex::SetPos(const VR::Vec4& parPos)
{
	pos_.x = parPos.x;
	pos_.y = parPos.y;
	pos_.z = parPos.z;
	pos_.w = parPos.w;
}
// ----------------------------------------------------------------------------
/*
Note: Q is symetric, v is a column vector

error(v) = v' * Q * v

			[q0 q1 q2 q3
			 q1 q4 q5 q6
			 q2 q5 q7 q8
			 q3 q6 q8 q9 ] 
[x y z w] x					=
[(x*q0 + y*q1 + z*q2 + w*q3), (x*q1 + y*q4 + z*q5 + w*q6), (x*q2 + y*q5 + z*q7 + w*q8), (x*q3 + y*q6 + z*q8 + w*q9)]

error(v) =	(x*q0 + y*q1 + z*q2 + w*q3)*x + 
			(x*q1 + y*q4 + z*q5 + w*q6)*y + 
			(x*q2 + y*q5 + z*q7 + w*q8)*z +
			(x*q3 + y*q6 + z*q8 + w*q9)*w
*/
float Vertex::QuadricError()
{
	assert(associatedQuadric_);

	if (quadricErrorComputed_)
		return quadricError_;

	float x = pos_.x;
	float y = pos_.y;
	float z = pos_.z;
	float w = pos_.w;
	const float* q = associatedQuadric_->Values();

	quadricError_ = 0.f;
	quadricError_ += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	quadricError_ += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	quadricError_ += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	quadricError_ += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;

	quadricErrorComputed_ = true;

	return quadricError_;
}
// ----------------------------------------------------------------------------
float Vertex::QuadricError() const
{
	assert(associatedQuadric_);
	assert(quadricErrorComputed_);
	
	return quadricError_;
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
