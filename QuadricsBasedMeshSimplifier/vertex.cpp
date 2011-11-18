
#include "vertex.h"
#include "vertex_pair.h"
#include "face.h"
#include <algorithm>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Vertex::Vertex(const VR::Vertex& parVertex, size_t parId) :
	pos_((double)parVertex.x, (double)parVertex.y, (double)parVertex.z, 1.0f),
	id_(parId),
	deleteMe_(false)
{
}
// ----------------------------------------------------------------------------
Vertex::~Vertex()
{
}
// ----------------------------------------------------------------------------
void Vertex::SetAssociatedQuadric(const Quadric& parQuadric)
{
	associatedQuadric_ = parQuadric;

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
			assert(false); // This vertex got a face where it doesn't participate
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

#ifdef _DEBUG
	curFace = incidentFaces_.begin();
	faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
		if ((*curFace)->IsDegenerated())
			break;
	assert(curFace == faceEnd);
#endif
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
void Vertex::RemoveInvalidPair(std::vector<VertexPair*>& parDeletePairs)
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
			parDeletePairs.push_back(*curPair);
		}

	// FIXME: Gerer le cas des vertex pair (non edge), ADJACENCE ATTENTION !!!!
	
	deletePairList::iterator curDelPair = deletePairs.begin();
	deletePairList::iterator delPairEnd = deletePairs.end();
	for (; curDelPair != delPairEnd; ++curDelPair)
		pairs_.erase(*curDelPair);
}
// ----------------------------------------------------------------------------
void Vertex::RemoveDuplicatedPair(std::vector<VertexPair*>& parDeletePairs)
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
				parDeletePairs.push_back(pair2);
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
void Vertex::UpdatePairPosAndQuadric(std::vector<VertexPair*>& parUpdatePairs)
{
	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
	{
		(*curPair)->ComputePosAndQuadric();

		parUpdatePairs.push_back(*curPair);
	}
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
