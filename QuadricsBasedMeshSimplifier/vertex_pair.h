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
#ifdef OPTIMIZE
	void Contract(std::vector<VertexPair*>& parDeletePairs, std::vector<VertexPair*>& parUpdatePairs);
#else
	void Contract();
#endif

public:
	void ComputePosAndQuadric();
	void ComputeQuadricError();
	bool IsDegenerated() const { return (v0_ == v1_); }
	void RemoveOnRelatedVertex();

public:
	inline bool DeleteMe() const { return deleteMe_; }
	void SetDeleteMe() { assert(!deleteMe_); deleteMe_ = true; }
	Vertex* V0() { return v0_; }
	Vertex* V1() { return v1_; }
	void SetVertices(Vertex* parV0, Vertex* parV1);
	double QuadricError() const { assert(quadricErrorComputed_ || deleteMe_); return quadricError_; }
	void UnsetQuadricErrorComputed() { assert(quadricErrorComputed_); quadricErrorComputed_ = false; }
#ifdef OPTIMIZE
	void AssignQuadricErrorWithNewValue() { assert(quadricErrorComputed_); quadricError_ = newQuadricError_; }
	size_t HeapInd() const { return heapInd_; }
	void SetHeapInd(size_t parInd) { heapInd_ = parInd; }
#endif

private:
	Vertex* v0_;
	Vertex* v1_;

	VR::Vec4 pos_;
	Quadric* quadric_;

	bool quadricErrorComputed_;
	double quadricError_;
#ifdef OPTIMIZE
	double newQuadricError_;
	size_t heapInd_;
#endif

	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 