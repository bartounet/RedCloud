#ifndef MESH_H_
#define MESH_H_


#include "../VertexRecolor/geometry.h"
#include "vertex.h"
#include "face.h"
#include "vertex_pair.h"
#ifdef OPTIMIZE
 #include "vertex_pair_heap.h"
#endif
#include <vector>
#include <set>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Vertex;
class Mesh
{
public:
	typedef std::pair<Vertex*, Vertex*> PairType;
	typedef std::vector<VertexPair*> PairListType;
	typedef PairListType::const_iterator PairListItType;

public:
	Mesh(const VR::Mesh& parVRMesh);
	~Mesh();

public:
	VR::Mesh* ExportToVRMesh();
	void ComputeInitialQuadrics();
	void SelectAndComputeVertexPairs();
	void Simplify(size_t parMaxFaces);

private:
	void GenerateAdjacency_();
#ifndef OPTIMIZE
	VertexPair* ExtractCostlessVertexPair_();
#endif
	size_t NbValidFaces_() const;

private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
	std::set<PairType> edges_;

#ifdef OPTIMIZE
	VertexPairHeap pairsHeap_;
#else
	PairListType pairs_;
#endif
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif 