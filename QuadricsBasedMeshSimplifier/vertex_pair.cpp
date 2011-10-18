
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
	v1_(parV1),
	quadricError_(0.0),
	quadricErrorComputed_(false),
	deleteMe_(false),
	quadric_(0)
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
void VertexPair::ComputePosAndQuadric()
{
	assert(!quadricErrorComputed_);

	// compute the position
	pos_ = v0_->Pos();  // FIXME: Try a more optimal position...

	// compute the new quadric
	if (quadric_)
		delete quadric_;
	quadric_ = new Quadric();
	quadric_->Add(v0_->AssociatedQuadric());
	quadric_->Add(v1_->AssociatedQuadric());
}
// ----------------------------------------------------------------------------
void VertexPair::ComputeQuadricError()
{
	assert(!quadricErrorComputed_);
	assert(quadric_);

	float x = pos_.x;
	float y = pos_.y;
	float z = pos_.z;
	float w = pos_.w;
	const float* q = quadric_->Values();

	quadricError_ = 0.0f;
	quadricError_ += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	quadricError_ += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	quadricError_ += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	quadricError_ += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;
	quadricError_ = std::max(0.0f, quadricError_);

	if (quadricError_ < 0.0)
		int a = 0;

	quadricErrorComputed_ = true;
}
// ----------------------------------------------------------------------------
void VertexPair::Contract()
{
	assert(!deleteMe_);
	assert(quadricErrorComputed_);

	v0_->SetPos(pos_);
	v0_->SetAssociatedQuadric(quadric_);

	v1_->UpdateIncidentFaces(v0_);
	v0_->AddIncidentFaces(v1_->IncidentFaces());
	v0_->RemoveDegeneratedFaces();

	v0_->AddPairs(v1_->Pairs());
	v0_->UpdatePairWithThis(v1_);

	v0_->RemoveDuplicatedPair();
	v0_->RemoveInvalidPair();

	v0_->UpdatePairPosAndQuadric();

	v1_->SetDeleteMe();
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

	quadricErrorComputed_ = false;
}
// ----------------------------------------------------------------------------
void VertexPair::RemoveOnRelatedVertex()
{
	assert(deleteMe_);

	v0_->RemovePair(this);
	v1_->RemovePair(this);
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================