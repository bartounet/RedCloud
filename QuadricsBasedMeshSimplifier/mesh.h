#ifndef MESH_H_
#define MESH_H_


#include "../VertexRecolor/geometry.h"
#include "vertex.h"
#include "face.h"
#include "vertex_pair_heap.h"
#include <vector>
#include <set>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Mesh
{
public:
	typedef std::pair<Vertex*, Vertex*> PairType;

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
	size_t NbValidFaces_() const;

private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
	std::set<PairType> edges_;

	VertexPairHeap pairsHeap_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif 