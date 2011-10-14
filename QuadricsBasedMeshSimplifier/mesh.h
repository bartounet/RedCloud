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
class Mesh
{
public:
	typedef std::pair<size_t, size_t> PairType;
	typedef std::priority_queue<VertexPair*, std::vector<VertexPair*>, VertexPair::Cmp> HeapType;

public:
	Mesh(const VR::Mesh& parVRMesh);
	~Mesh();

#if 0
public:
	const std::vector<Vertex>& Vertices() const { return vertices_; }
	const std::vector<Face>& Faces() const { return faces_; }
	const std::set<EdgeType>& Edges() const { return edges_; }
#endif

public:
	VR::Mesh* ExportToVRMesh() const;
	void ComputeInitialQuadrics();
	void SelectAndComputeVertexPairs();

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