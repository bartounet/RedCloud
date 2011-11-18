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
	void Contract(std::vector<VertexPair*>& parDeletePairs, std::vector<VertexPair*>& parUpdatePairs);

public:
	void ComputePosAndQuadric();
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
	void AssignQuadricErrorWithNewValue() { assert(quadricErrorComputed_); quadricError_ = newQuadricError_; }
	size_t HeapInd() const { return heapInd_; }
	void SetHeapInd(size_t parInd) { heapInd_ = parInd; }

private:
	void ComputeOptimalPos_();
	double ComputeQuadricError_(const VR::Vec4& parPos) const;

private:
	Vertex* v0_;
	Vertex* v1_;

	VR::Vec4 pos_;
	Quadric quadric_;

	bool quadricErrorComputed_;
	double quadricError_;

	double newQuadricError_;
	size_t heapInd_;

	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 