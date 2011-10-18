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
	void Contract();

public:
	void ComputePosAndQuadric();
	void ComputeQuadricError();
	bool IsDegenerated() const { return (v0_ == v1_); }

public:
	bool DeleteMe() const { return deleteMe_; }
	void SetDeleteMe() { assert(!deleteMe_); deleteMe_ = true; }
	void UnsetQuadricErrorComputed() { assert(quadricErrorComputed_); quadricErrorComputed_ = false; }
	Vertex* V0() { return v0_; }
	Vertex* V1() { return v1_; }
	void SetVertices(Vertex* parV0, Vertex* parV1);
	void RemoveOnRelatedVertex();
	double QuadricError() const { assert(quadricErrorComputed_); return quadricError_; }

private:
	Vertex* v0_;
	Vertex* v1_;

	VR::Vec4 pos_;
	Quadric* quadric_;

	bool quadricErrorComputed_;
	double quadricError_;

	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 