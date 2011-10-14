
#include "vertex_pair.h"
#include "quadric.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
VertexPair::VertexPair(Vertex* parV0, Vertex* parV1) :
	v0_(parV0),
	v1_(parV1)
{
	assert(v0_);
	assert(v1_);

	ComputeOptimalVertex_();
}
// ----------------------------------------------------------------------------
VertexPair::~VertexPair()
{
}
// ----------------------------------------------------------------------------
void VertexPair::ComputeOptimalVertex_()
{
	// compute the position
	optimalVertex_.SetPos(v0_->Pos()); // FIXME: Try a more optimal position...

	// compute the new quadric
	Quadric* quadric = new Quadric();
	quadric->Add(v0_->AssociatedQuadric());
	quadric->Add(v1_->AssociatedQuadric());
	optimalVertex_.SetAssociatedQuadric(quadric);
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================