
#include "mesh.h"
#include <assert.h>
#include <stdio.h>


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
	printf("[ ] Construction Mesh from VRMesh\n");

	size_t nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);

	printf("\t[ ] Copying Vertices\n");
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex]));
	printf("\t[+] Vertices copied\n");

	printf("\t[ ] Copying Faces\n");
	size_t nbFaces = parVRMesh.faces.size();
	assert(nbFaces > 0);
	for (size_t curFace = 0; curFace < nbFaces; ++curFace)
		faces_.push_back(Face(parVRMesh.faces[curFace]));
	printf("\t[+] Faces copied\n");

	GenerateAdjacency_();

	printf("[+] Mesh constructed\n");
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

	printf("\t[ ] Generating adjacency\n");

	// incident face
	for (size_t curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face* face = &faces_[curFace];
		vertices_[face->V0()].AddIncidentFace(face);
		vertices_[face->V1()].AddIncidentFace(face);
		vertices_[face->V2()].AddIncidentFace(face);
	}

	printf("\t[+] Adjacency generated\n");
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
		newMesh->faces.push_back(VR::Face(faces_[curFace].V0(), 
			faces_[curFace].V1(), faces_[curFace].V2()));
	}

	return newMesh;
}
// ----------------------------------------------------------------------------
void Mesh::ComputeInitialQuadrics()
{
	printf("[ ] Computing initial quadrics\n");

	for (size_t curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& vertex = vertices_[curVertex];
		std::vector<VR::Vec4> planes;

		// for each incident faces
		for (size_t curFace = 0; curFace < vertex.IncidentFaces().size(); ++curFace)
		{
			const Face* face = vertex.IncidentFaces()[curFace];

			const Vertex& v0 = vertices_[face->V0()];
			const Vertex& v1 = vertices_[face->V1()];
			const Vertex& v2 = vertices_[face->V2()];

			VR::Vec4 edge1(v0.Pos(), v1.Pos());
			VR::Vec4 edge2(v0.Pos(), v2.Pos());

			VR::Vec4 normal = VR::Vec4::CrossProduct(edge1, edge2);
			normal = VR::Vec4::Normalize(normal);

			float dPlaneComponent = normal.x * vertex.Pos().x;
			dPlaneComponent += normal.y * vertex.Pos().y;
			dPlaneComponent += normal.z * vertex.Pos().z;
			dPlaneComponent *= -1.f;
			VR::Vec4 newPlane(normal.x, normal.y, normal.z, dPlaneComponent);

			planes.push_back(newPlane);
		}

		// FIXME : Fill the quadric
	}

	printf("[+] Initial quadrics computed\n");
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================