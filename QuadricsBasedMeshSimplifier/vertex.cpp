
#include "vertex.h"
#include "../VertexRecolor/types.h"
#include "vertex_pair.h"
#include <vector>
#include <algorithm>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
#if 0
Vertex::Vertex(float parX, float parY, float parZ) :
	pos_(parX, parY, parZ, 1.0f),
	associatedQuadric_(0),
	id_((size_t) -1),
	deleteMe_(false)	
#if 0
	,quadricErrorComputed_(false),
	quadricError_(0.f)
#endif
{
}
#endif
// ----------------------------------------------------------------------------
Vertex::Vertex(const VR::Vertex& parVertex, size_t parId) :
	pos_((double)parVertex.x, (double)parVertex.y, (double)parVertex.z, 1.0f),
	associatedQuadric_(0),
	id_(parId),
	deleteMe_(false)
#if 0
	,quadricErrorComputed_(false),
	quadricError_(0.f)
#endif
{
}
// ----------------------------------------------------------------------------
#if 0
Vertex::Vertex() :
	pos_(0.0f, 0.0f, 0.0f, 1.0f),
	associatedQuadric_(0),
	id_((size_t) -1),
	deleteMe_(false)
#if 0
	,quadricErrorComputed_(false),
	quadricError_(0.f)
#endif
{
}
#endif
// ----------------------------------------------------------------------------
Vertex::~Vertex()
{
	if (associatedQuadric_)
		delete associatedQuadric_;
}
// ----------------------------------------------------------------------------
void Vertex::SetAssociatedQuadric(Quadric* parQuadric)
{
	assert(parQuadric);

	if (associatedQuadric_)
		delete associatedQuadric_;
	associatedQuadric_ = parQuadric;
#if 0
	quadricErrorComputed_ = false;
#endif

	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
		(*curPair)->UnsetQuadricErrorComputed();
}
// ----------------------------------------------------------------------------
void Vertex::SetPos(const VR::Vec4& parPos)
{
	pos_.x = parPos.x;
	pos_.y = parPos.y;
	pos_.z = parPos.z;
	pos_.w = parPos.w;
}
// ----------------------------------------------------------------------------
// The degenerated faces are not added
void Vertex::AddIncidentFaces(const FaceListType& parFaceList)
{
//	assert(parFaceList.size() > 0);

	FaceListType::const_iterator curFace = parFaceList.begin();
	FaceListType::const_iterator faceEnd = parFaceList.end();

	for (; curFace != faceEnd; ++curFace)
		if (!(*curFace)->IsDegenerated())
			incidentFaces_.push_back(*curFace);
}
// ----------------------------------------------------------------------------
void Vertex::UpdateIncidentFaces(Vertex* parNewVertex)
{
	assert(parNewVertex);
	
	FaceListType::const_iterator curFace = incidentFaces_.begin();
	FaceListType::const_iterator faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
	{
		Face* face = *curFace;

		if (face->V0() == this)
			face->SetV0(parNewVertex);
		else if (face->V1() == this)
			face->SetV1(parNewVertex);
		else if (face->V2() == this)
			face->SetV2(parNewVertex);
		else
			assert(false); /// WTF !!!!
	}
}
// ----------------------------------------------------------------------------
void Vertex::AddIncidentFace(Face* parFace)
{
	assert(parFace);
	incidentFaces_.push_back(parFace);
}
// ----------------------------------------------------------------------------
void Vertex::AddPair(VertexPair* parPair)
{
	assert(parPair);
	pairs_.push_back(parPair);
}
// ----------------------------------------------------------------------------
void Vertex::AddPairs(const PairListType& parPairList)
{
	assert(parPairList.size() > 0);

	PairListType::const_iterator curPair = parPairList.begin();
	PairListType::const_iterator pairEnd = parPairList.end();
	for (; curPair != pairEnd; ++curPair)
		pairs_.push_back(*curPair);
}
// ----------------------------------------------------------------------------
// We only remove degenerated face
void Vertex::RemoveIncidentFace(Face* parFace)
{
	assert(parFace);
	assert(parFace->IsDegenerated());

	FaceListType::const_iterator faceIt = std::find(incidentFaces_.begin(), incidentFaces_.end(), parFace);
	assert(faceIt != incidentFaces_.end()); // we are deleting a non existing face

	incidentFaces_.erase(faceIt);
}
// ----------------------------------------------------------------------------
void Vertex::RemoveDegeneratedFaces()
{
	FaceListType facesToDelete;

	FaceListType::const_iterator curFace = incidentFaces_.begin();
	FaceListType::const_iterator faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
		if ((*curFace)->IsDegenerated())
			facesToDelete.push_back(*curFace);

	curFace = facesToDelete.begin();
	faceEnd = facesToDelete.end();
	for (; curFace != faceEnd; ++curFace)
		(*curFace)->RemoveOnRelatedVertex();

	// debug only
	curFace = incidentFaces_.begin();
	faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
		if ((*curFace)->IsDegenerated())
			break;
	assert(curFace == faceEnd);
	// debug only
}
// ----------------------------------------------------------------------------
void Vertex::UpdatePairWithThis(const Vertex* parOldVertex)
{
	assert(parOldVertex);

	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
	{
		VertexPair* pair = *curPair;

		if (pair->V0() == parOldVertex)
			pair->SetVertices(this, pair->V1());
		else if (pair->V1() == parOldVertex)
			pair->SetVertices(this, pair->V0());
	}
}
// ----------------------------------------------------------------------------
void Vertex::RemoveInvalidPair()
{
	typedef std::vector<PairListType::iterator> deletePairList;
	deletePairList deletePairs;

	PairListType::iterator curPair = pairs_.begin();
	PairListType::iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
		if ((*curPair)->IsDegenerated())
		{
			(*curPair)->SetDeleteMe();
			deletePairs.push_back(curPair);
		}

	// FIXME: Gerer le cas des vertex pair (non edge), ADJACENCE ATTENTION !!!!
	
	deletePairList::iterator curDelPair = deletePairs.begin();
	deletePairList::iterator delPairEnd = deletePairs.end();
	for (; curDelPair != delPairEnd; ++curDelPair)
		pairs_.erase(*curDelPair);
}
// ----------------------------------------------------------------------------
void Vertex::RemoveDuplicatedPair()
{
	PairListType deletePairs;

	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != (pairEnd - 1); ++curPair)
	{
		PairListType::const_iterator curPair2 = curPair + 1;
		for (; curPair2 != pairEnd; ++curPair2)
		{
			VertexPair* pair1 = *curPair;
			VertexPair* pair2 = *curPair2;

			if ((pair1->V0() == pair2->V0()) && (pair1->V1() == pair2->V1()))
			{
				pair2->SetDeleteMe();
				deletePairs.push_back(pair2);
			}
		}
	}

	curPair = deletePairs.begin();
	pairEnd = deletePairs.end();
	for (; curPair != pairEnd; ++curPair)
		(*curPair)->RemoveOnRelatedVertex();

	// FIXME: Debug Check
}
// ----------------------------------------------------------------------------
void Vertex::RemovePair(VertexPair* parPair)
{
	assert(parPair);
	assert(parPair->DeleteMe());

	PairListType::const_iterator pairIt = std::find(pairs_.begin(), pairs_.end(), parPair);
	assert(pairIt != pairs_.end());

	pairs_.erase(pairIt);
}
// ----------------------------------------------------------------------------
void Vertex::UpdatePairPosAndQuadric()
{
	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
	{
		(*curPair)->ComputePosAndQuadric();
		(*curPair)->ComputeQuadricError();
	}
}
// ----------------------------------------------------------------------------
#if 0
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
float Vertex::QuadricError()
{
	assert(associatedQuadric_);

	if (quadricErrorComputed_)
		return quadricError_;

	float x = pos_.x;
	float y = pos_.y;
	float z = pos_.z;
	float w = pos_.w;
	const float* q = associatedQuadric_->Values();

	quadricError_ = 0.f;
	quadricError_ += (x*q[0] + y*q[1] + z*q[2] + w*q[3]) * x;
	quadricError_ += (x*q[1] + y*q[4] + z*q[5] + w*q[6]) * y;
	quadricError_ += (x*q[2] + y*q[5] + z*q[7] + w*q[8]) * z;
	quadricError_ += (x*q[3] + y*q[6] + z*q[8] + w*q[9]) * w;

	quadricErrorComputed_ = true;

	return quadricError_;
}
// ----------------------------------------------------------------------------
float Vertex::QuadricError() const
{
	assert(associatedQuadric_);
	assert(quadricErrorComputed_);
	
	return quadricError_;
}
#endif
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
