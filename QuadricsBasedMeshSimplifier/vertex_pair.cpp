
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
#ifdef OPTIMIZE
	, newQuadricError_(-1.0)
#endif
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
	if (!quadric_)
		quadric_ = new Quadric();
	else
		quadric_->Init();
	quadric_->Add(v0_->AssociatedQuadric());
	quadric_->Add(v1_->AssociatedQuadric());
}
// ----------------------------------------------------------------------------
void VertexPair::ComputeQuadricError()
{
	assert(!quadricErrorComputed_);
	assert(quadric_);

	double x = pos_.x;
	double y = pos_.y;
	double z = pos_.z;
	double w = pos_.w;
	const double* q = quadric_->Values();

	double error = 0.0;
	error += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	error += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	error += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	error += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;
	if (error < 0.0) // floating round error hack
		error = 0.0;

#ifdef OPTIMIZE
	newQuadricError_ = error;
#else
	quadricError_ = error;
#endif
	
	quadricErrorComputed_ = true;
}
// ----------------------------------------------------------------------------
#ifdef OPTIMIZE
void VertexPair::Contract(std::vector<VertexPair*>& parDeletePairs,
						  std::vector<VertexPair*>& parUpdatePairs)
#else
void VertexPair::Contract()
#endif
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

#ifdef OPTIMIZE
	v0_->RemoveDuplicatedPair(parDeletePairs);
	v0_->RemoveInvalidPair(parDeletePairs);
	v0_->UpdatePairPosAndQuadric(parUpdatePairs);
#else
	v0_->RemoveDuplicatedPair();
	v0_->RemoveInvalidPair();
	v0_->UpdatePairPosAndQuadric();
#endif

	v1_->SetDeleteMe();
	deleteMe_ = true;
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