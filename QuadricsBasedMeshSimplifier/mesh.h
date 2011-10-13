#ifndef MESH_H_
#define MESH_H_


#include "../VertexRecolor/geometry.h"
#include "vertex.h"
#include "face.h"
#include <vector>
#include <set>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Mesh
{
public:
	typedef std::pair<size_t, size_t> EdgeType;

public:
	Mesh();
	Mesh(const VR::Mesh& parVRMesh);
	~Mesh();

public:
	const std::vector<Vertex>& Vertices() const { return vertices_; }
	const std::vector<Face>& Faces() const { return faces_; }
	const std::set<EdgeType>& Edges() const { return edges_; }

public:
	VR::Mesh* ExportToVRMesh() const;
	void ComputeInitialQuadrics();
	void SelectValidPairs();

private:
	void GenerateAdjacency_();

private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
	std::set<EdgeType> edges_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 