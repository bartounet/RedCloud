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
	struct Cmp
	{
		bool operator()(VertexPair* parLeft, VertexPair* parRight)
		{
			return (parLeft->OptimalVertex().QuadricError() > 
				parRight->OptimalVertex().QuadricError());
		}
	};

public:
	VertexPair(Vertex* parV0, Vertex* parV1);
	~VertexPair();

public:
	void Contract();

public:
	const Vertex& OptimalVertex() const { return optimalVertex_; }

private:
	void ComputeOptimalVertex_();

private:
	Vertex* v0_;
	Vertex* v1_;
	Vertex optimalVertex_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 