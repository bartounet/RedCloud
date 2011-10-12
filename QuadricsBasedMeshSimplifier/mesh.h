#ifndef MESH_H_
#define MESH_H_


#include "../VertexRecolor/geometry.h"
#include "vertex.h"
#include "edge.h"
#include "face.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Mesh
{
public:
	Mesh();
	Mesh(const VR::Mesh& parVRMesh);
	~Mesh();

public:
	const std::vector<Vertex>& Vertices() const { return vertices_; }
	const std::vector<Edge>& Edges() const { return edges_; }
	const std::vector<Face>& Faces() const { return faces_; }

public:
	VR::Mesh* ExportToVRMesh() const;
	void ComputeInitialQuadrics();

private:
	void GenerateAdjacency_();

private:
	std::vector<Vertex> vertices_;
	std::vector<Edge> edges_;
	std::vector<Face> faces_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 