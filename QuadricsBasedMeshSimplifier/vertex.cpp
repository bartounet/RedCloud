
#include "vertex.h"
#include "vertex_pair.h"
#include "face.h"
#include <algorithm>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Vertex::Vertex(const VR::Vertex& parVertex, uint parId) :
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
#ifdef _DEBUG // Non-debug version is inlined
void Vertex::SetQuadric(const Quadric& parQuadric)
{
	quadric_ = parQuadric;

	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
		(*curPair)->UnsetQuadricErrorComputed();
}
#endif
// ----------------------------------------------------------------------------
// The degenerated faces must not be added
void Vertex::AddIncidentFaces(const FaceListType& parFaceList)
{
	FaceListType::const_iterator curFace = parFaceList.begin();
	FaceListType::const_iterator faceEnd = parFaceList.end();

	for (; curFace != faceEnd; ++curFace)
	{
		assert(!(*curFace)->IsDegenerated());
		incidentFaces_.push_back(*curFace);
	}
}
// ----------------------------------------------------------------------------
void Vertex::ReplaceThisInIncidentFacesWith(Vertex* parNewVertex)
{
	assert(parNewVertex);
	
	FaceListType::const_iterator curFace = incidentFaces_.begin();
	FaceListType::const_iterator faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
	{
		Face& face = *(*curFace);
		assert(!face.IsDegenerated());

		if (face.V0() == this)
			face.SetV0(parNewVertex);
		else if (face.V1() == this)
			face.SetV1(parNewVertex);
		else if (face.V2() == this)
			face.SetV2(parNewVertex);
		else
			assert(false); // This vertex got a face where it doesn't participate
	}
}
// ----------------------------------------------------------------------------
void Vertex::AddIncidentFace(Face* parFace)
{
	assert(parFace);
	assert(!parFace->IsDegenerated());

	incidentFaces_.push_back(parFace);
}
// ----------------------------------------------------------------------------
void Vertex::AddPair(VertexPair* parPair)
{
	assert(parPair);
	assert(!parPair->IsDegenerated());

	pairs_.push_back(parPair);
}
// ----------------------------------------------------------------------------
void Vertex::AddPairs(const PairListType& parPairList)
{
	PairListType::const_iterator curPair = parPairList.begin();
	for (; curPair != parPairList.end(); ++curPair)
	{
		assert(!(*curPair)->IsDegenerated());
		pairs_.push_back(*curPair);
	}
}
// ----------------------------------------------------------------------------
// Only degenerated face can be removed
void Vertex::RemoveIncidentFace(Face* parFace)
{
	assert(parFace);
	assert(parFace->IsDegenerated());

	FaceListType::iterator faceIt = std::find(incidentFaces_.begin(), incidentFaces_.end(), parFace);
	assert(faceIt != incidentFaces_.end()); // we are deleting a non existing face

	incidentFaces_.erase(faceIt);
}
// ----------------------------------------------------------------------------
uint Vertex::RemoveDegeneratedFaces()
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
	{
		(*curFace)->RemoveOnRelatedVertex();
		RemoveIncidentFace(*curFace);
	}

#ifdef _DEBUG
	curFace = incidentFaces_.begin();
	faceEnd = incidentFaces_.end();
	for (; curFace != faceEnd; ++curFace)
		if ((*curFace)->IsDegenerated())
			break;
	assert(curFace == faceEnd);
#endif

	return facesToDelete.size();
}
// ----------------------------------------------------------------------------
void Vertex::ReplaceThisInPairsWith(Vertex* parNewVertex)
{
	assert(parNewVertex);

	PairListType::const_iterator curPair = pairs_.begin();
	PairListType::const_iterator pairEnd = pairs_.end();
	for (; curPair != pairEnd; ++curPair)
	{
		VertexPair& pair = *(*curPair);

		if (pair.V0() == this)
		{
			assert(pair.V1() != this);
			pair.SetVertices(parNewVertex, pair.V1());
		}
		else if (pair.V1() == this)
		{
			assert(pair.V0() != this);
			pair.SetVertices(parNewVertex, pair.V0());
		}
		else
			assert(false); // this vertex doesn't appear in pair
	}
}
// ----------------------------------------------------------------------------
void Vertex::RemoveDegeneratedPairs(std::vector<VertexPair*>& parDeletePairs)
{
	PairListType pairsToDelete;

	PairListType::const_iterator curPair = pairs_.begin();
	for (; curPair != pairs_.end(); ++curPair)
		if ((*curPair)->IsDegenerated())
			pairsToDelete.push_back(*curPair);

	curPair = pairsToDelete.begin();
	for (; curPair != pairsToDelete.end(); ++curPair)
	{
		VertexPair* pair = *curPair;

		pair->SetDeleteMe();
		if (!pair->IsDegenerated())
			pair->RemoveOnRelatedVertex();
		else
		{
			assert(pair->IsDegenerated());
			pair->V0()->RemovePair(pair);
		}

		parDeletePairs.push_back(pair);
	}

#ifdef _DEBUG
	curPair = pairs_.begin();
	for (; curPair != pairs_.end(); ++curPair)
		if ((*curPair)->IsDegenerated())
			break;
	assert(curPair == pairs_.end());
#endif
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
	if (pairs_.size() == 0)
		return;

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

	// FIXME: Debug Check (unicité des pairs)
}
// ----------------------------------------------------------------------------
void Vertex::RemovePair(VertexPair* parPair)
{
	assert(parPair);
	assert(parPair->DeleteMe());

	PairListType::iterator pairIt = std::find(pairs_.begin(), pairs_.end(), parPair);
	assert(pairIt != pairs_.end());

	pairs_.erase(pairIt);
}
// ----------------------------------------------------------------------------
void Vertex::UpdatePairPosAndQuadric(std::vector<VertexPair*>& parUpdatePairs)
{
	for (uint curPair = 0; curPair < pairs_.size(); ++curPair)
	{
		VertexPair* pair = pairs_[curPair];
		assert(!pair->DeleteMe());
		assert(!pair->IsDegenerated());

		pair->ComputePosAndQuadric();

		parUpdatePairs.push_back(pair);
	}
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
