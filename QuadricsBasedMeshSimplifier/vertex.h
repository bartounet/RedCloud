#ifndef VERTEX_H_
#define VERTEX_H_


#include "../VertexRecolor/types.h"
#include "../VertexRecolor/geometry.h"
#include "quadric.h"
#include <vector>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class VertexPair;
class Face;
class Vertex
{
public:
	typedef std::vector<VertexPair*> PairListType;
	typedef std::vector<Face*> FaceListType;

public:
	Vertex(const VR::Vertex& parVertex, uint parId);
	~Vertex();

public:
	void AddIncidentFace(Face* parFace);
	void AddIncidentFaces(const FaceListType& parFaceList);
	void AddPair(VertexPair* parPair);
	void AddPairs(const PairListType& parPairList);

	void UpdateIncidentFaces(Vertex* parNewVertex);
	void UpdatePairWithThis(const Vertex* parOldVertex);
	void UpdatePairPosAndQuadric(std::vector<VertexPair*>& parUpdatePairs);

	void RemoveIncidentFace(Face* parFace);
	void RemoveDegeneratedFaces();
	void RemovePair(VertexPair* parPair);
	void RemoveInvalidPair(std::vector<VertexPair*>& parDeletePairs);
	void RemoveDuplicatedPair(std::vector<VertexPair*>& parDeletePairs);

public:
	inline const VR::Vec4& Pos() const { return pos_; }
	inline const FaceListType& IncidentFaces() const { return incidentFaces_; }
	inline const Quadric& GetQuadric() const { return quadric_; }
	inline uint Id() const { return id_; }
	inline const PairListType& Pairs() const { return pairs_; }
	inline bool DeleteMe() const { return deleteMe_; }

public:
	inline void SetId(uint parId) { id_ = parId; }
	inline void SetDeleteMe() { deleteMe_ = true; }
	inline void SetPos(const VR::Vec4& parPos) { pos_ = parPos; }
#ifdef _DEBUG
	void SetQuadric(const Quadric& parQuadric);
#else
	inline void SetQuadric(const Quadric& parQuadric) { quadric_ = parQuadric; }
#endif

private:
	FaceListType incidentFaces_;
	PairListType pairs_;

	VR::Vec4 pos_;
	Quadric quadric_;
	
	uint id_;
	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif