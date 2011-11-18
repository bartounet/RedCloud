
#include "vertex_pair.h"
#include "quadric.h"
#include <assert.h>
#include <limits>
#include <algorithm>


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
	newQuadricError_(-1.0)
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
void VertexPair::ComputeOptimalPos_()
{
	assert(!quadricErrorComputed_);

	// FIXME: Rendre ce code plus classe et gerer le point optimal !
	const VR::Vec4& pos0 = v0_->Pos();
	const VR::Vec4& pos1 = v1_->Pos();
	VR::Vec4 midPos;
	midPos.x = (pos0.x + pos1.x) / 2;
	midPos.y = (pos0.y + pos1.y) / 2;
	midPos.z = (pos0.z + pos1.z) / 2;

	pos_ = pos0;
	double minError = ComputeQuadricError_(pos0);
	double error = ComputeQuadricError_(midPos);
	if (error < minError)
	{
		pos_ = midPos;
		minError = error;
	}

	error = ComputeQuadricError_(pos1);
	if (error < minError)
	{
		pos_ = pos1;
		minError = error;
	}

	newQuadricError_ = minError;
	quadricErrorComputed_ = true;
}
// ----------------------------------------------------------------------------
void VertexPair::ComputePosAndQuadric()
{
	assert(!quadricErrorComputed_);

	quadric_.Init();
	quadric_.Add(v0_->AssociatedQuadric());
	quadric_.Add(v1_->AssociatedQuadric());

	ComputeOptimalPos_();
}
// ----------------------------------------------------------------------------
double VertexPair::ComputeQuadricError_(const VR::Vec4& parPos) const
{
	assert(!quadricErrorComputed_);

	double x = parPos.x;
	double y = parPos.y;
	double z = parPos.z;
	double w = parPos.w;
	const double* q = quadric_.Values();

	double error = 0.0;
	error += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	error += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	error += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	error += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;
	if (error < 0.0) // floating round error hack
		error = 0.0;

	return error;
}
// ----------------------------------------------------------------------------
void VertexPair::Contract(std::vector<VertexPair*>& parDeletePairs, std::vector<VertexPair*>& parUpdatePairs)
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

	v0_->RemoveDuplicatedPair(parDeletePairs);
	v0_->RemoveInvalidPair(parDeletePairs);
	v0_->UpdatePairPosAndQuadric(parUpdatePairs);

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