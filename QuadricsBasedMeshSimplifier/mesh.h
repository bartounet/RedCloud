#ifndef MESH_H_
#define MESH_H_


#include "../VertexRecolor/geometry.h"
#include "vertex.h"
#include "face.h"
#include "vertex_pair.h"
#include <vector>
#include <set>
#include <queue>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Vertex;
class Mesh
{
public:
	typedef std::pair<Vertex*, Vertex*> PairType;
	typedef std::priority_queue<VertexPair*, std::vector<VertexPair*>, VertexPair::Cmp> HeapType;

public:
	Mesh(const VR::Mesh& parVRMesh);
	~Mesh();

public:
	VR::Mesh* ExportToVRMesh() const;
	void ComputeInitialQuadrics();
	void SelectAndComputeVertexPairs();
	void Simplify(); // FIXME: stop criterions in args

private:
	void GenerateAdjacency_();

private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
	std::set<PairType> edges_;
	HeapType pairs_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 