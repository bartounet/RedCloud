#ifndef VERTEX_H_
#define VERTEX_H_


#include "../VertexRecolor/types.h"
#include "../VertexRecolor/geometry.h"
#include "face.h"
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
class Vertex
{
public:
	typedef std::vector<VertexPair*> PairListType;
	typedef std::vector<Face*> FaceListType;

public:
	Vertex();
	Vertex(double parX, double parY, double parZ);
	Vertex(const VR::Vertex& parVertex, size_t parId);
	~Vertex();

public:
	void AddIncidentFace(Face* parFace);
	void AddIncidentFaces(const FaceListType& parFaceList);
	void AddPair(VertexPair* parPair);
	void AddPairs(const PairListType& parPairList);
#if 0
	float QuadricError();
	float QuadricError() const;
#endif
	void UpdateIncidentFaces(Vertex* parNewVertex);
	void RemoveIncidentFace(Face* parFace);
	void RemoveDegeneratedFaces();
	void UpdatePairWithThis(const Vertex* parOldVertex);
	void RemoveInvalidPair();
	void RemoveDuplicatedPair();
	void RemovePair(VertexPair* parPair);
	void UpdatePairPosAndQuadric();

public:
	const VR::Vec4& Pos() const { return pos_; }
	const FaceListType& IncidentFaces() const { return incidentFaces_; }
	const Quadric& AssociatedQuadric() const { assert(associatedQuadric_); return *associatedQuadric_; }
	size_t Id() const { return id_; }
	const PairListType& Pairs() const { return pairs_; }
	bool DeleteMe() const { return deleteMe_; }

public:
	void SetAssociatedQuadric(Quadric* parQuadric);
	void SetPos(const VR::Vec4& parPos);
	void SetId(size_t parId) { id_ = parId; }
	void SetDeleteMe() { deleteMe_ = true; }

private:
	FaceListType incidentFaces_;
	PairListType pairs_;

	VR::Vec4 pos_;
	Quadric* associatedQuadric_;
#if 0
	bool quadricErrorComputed_;
	float quadricError_;
#endif
	
	size_t id_;
	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif