
#include "mesh.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
Mesh::Mesh()
{
}
// ----------------------------------------------------------------------------
Mesh::Mesh(const VR::Mesh& parVRMesh)
{
	size_t nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);

	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex]));

	size_t nbFaces = parVRMesh.faces.size();
	assert(nbFaces > 0);

	for (size_t curFace = 0; curFace < nbFaces; ++curFace)
		faces_.push_back(Face(parVRMesh.faces[curFace]));

	GenerateAdjacency_();
}
// ----------------------------------------------------------------------------
Mesh::~Mesh()
{
}
// ----------------------------------------------------------------------------
void Mesh::GenerateAdjacency_()
{
	assert(edges_.size() == 0);
	assert(vertices_.size() > 0);
	assert(faces_.size() > 0);

	// FIXME
}
// ----------------------------------------------------------------------------
VR::Mesh* Mesh::ExportToVRMesh() const
{
	VR::Mesh* newMesh = new VR::Mesh();

	size_t nbVertices = vertices_.size();
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
		newMesh->vertices.push_back(VR::Vertex(vertices_[curVertex].Pos()));

	size_t nbFaces = faces_.size();
	for (size_t curFace = 0; curFace < nbFaces; ++curFace)
	{
		newMesh->faces.push_back(VR::Face(faces_[curFace].V0(), faces_[curFace].V1(),
			faces_[curFace].V2()));
	}

	return newMesh;
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================