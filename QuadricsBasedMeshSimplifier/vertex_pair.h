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
	double ComputeQuadricError_(const VR::Vec4& parPos) const;

private:
	Vertex* v0_;
	Vertex* v1_;
	VR::Vec4 pos_;
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