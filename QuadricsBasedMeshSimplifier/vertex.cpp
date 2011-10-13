
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
	associatedQuadric_(0)
{
}
// ----------------------------------------------------------------------------
Vertex::Vertex(const VR::Vertex& parVertex) :
	pos_(parVertex.x, parVertex.y, parVertex.z, 1.f),
	associatedQuadric_(0)
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
float Vertex::QuadricError() const
{
	assert(associatedQuadric_);

	float x = pos_.x;
	float y = pos_.y;
	float z = pos_.z;
	float w = pos_.w;
	const float* q = associatedQuadric_->Values();

	float error = 0.f;
	error += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	error += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	error += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	error += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;

	return error;
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
