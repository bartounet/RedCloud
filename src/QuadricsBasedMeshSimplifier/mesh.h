#ifndef MESH_H_
#define MESH_H_


#include "../common/geometry.h"
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
	Mesh(const Com::Mesh& parVRMesh);
	~Mesh();

public:
	void ExportToVRMesh(Com::Mesh& parDstMesh) const;
	void ComputeInitialQuadrics();
	void SelectAndComputeVertexPairs();
	void Simplify(uint parMaxFaces);
	void Clean();
	void SetDeleteUnusedVerticesAndReassignVerticesId();
	void DeleteFacesIFN();
	void DeleteVerticesIFN();

public:
	bool HasZeroAreaSurfaceFaces() const; // can be slow

private:
	void GenerateAdjacency_();
	uint NbValidFaces_() const;

private:
	std::vector<Vertex*> vertices_;
	std::vector<Face*> faces_;

	VertexPairHeap pairsHeap_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif 