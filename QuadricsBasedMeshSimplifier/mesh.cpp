
#include "mesh.h"
#include "vertex.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <stdlib.h> // FIXME: delete me
#include <limits>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
Mesh::Mesh(const VR::Mesh& parVRMesh)
{
	printf("[ ] Constructing Mesh from VRMesh\n");

	size_t nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);
	printf("\t[ ] Copying %d Vertices\n", nbVertices);
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex], curVertex));
	printf("\t[+] Vertices copied\n");

	size_t nbFaces = parVRMesh.faces.size();
	assert(nbFaces > 0);
	printf("\t[ ] Copying %d Faces\n", nbFaces);
	for (size_t curFace = 0; curFace < nbFaces; ++curFace)
	{
		Vertex* v0 = &vertices_[parVRMesh.faces[curFace].vertices[0]];
		Vertex* v1 = &vertices_[parVRMesh.faces[curFace].vertices[1]];
		Vertex* v2 = &vertices_[parVRMesh.faces[curFace].vertices[2]];
		faces_.push_back(Face(v0, v1, v2));
	}
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

	// incident face + edges
	for (size_t curFace = 0; curFace < faces_.size(); ++curFace)
	{
		Face* face = &faces_[curFace];

		face->V0()->AddIncidentFace(face);
		face->V1()->AddIncidentFace(face);
		face->V2()->AddIncidentFace(face);

		edges_.insert(PairType(std::min(face->V0(), face->V1()), std::max(face->V0(), face->V1())));
		edges_.insert(PairType(std::min(face->V0(), face->V2()), std::max(face->V0(), face->V2())));
		edges_.insert(PairType(std::min(face->V1(), face->V2()), std::max(face->V1(), face->V2())));
	}

	printf("\t[+] Adjacency generated\n");
}
// ----------------------------------------------------------------------------
VR::Mesh* Mesh::ExportToVRMesh()
{
	VR::Mesh* newMesh = new VR::Mesh();

	size_t deleteOffset = 0;
	size_t nbVertices = vertices_.size();
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
	{
		Vertex* vertex = &vertices_[curVertex];

		// FIXME !!!!
#if 0
		if (vertex->DeleteMe()) 
			deleteOffset++;
		else
#endif
		{
			newMesh->vertices.push_back(VR::Vertex(vertices_[curVertex].Pos()));
			vertex->SetId(vertex->Id() - deleteOffset);
		}
	}

	size_t nbFaces = faces_.size();
	for (size_t curFace = 0; curFace < nbFaces; ++curFace)
	{
		Face* face = &faces_[curFace];

		if (!face->IsDegenerated())
		{
			newMesh->faces.push_back(VR::Face(faces_[curFace].V0()->Id(), 
				faces_[curFace].V1()->Id(), faces_[curFace].V2()->Id()));
		}
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

		// for each incident faces, compute the plane
		for (size_t curFace = 0; curFace < vertex.IncidentFaces().size(); ++curFace)
		{
			const Face* face = vertex.IncidentFaces()[curFace];

			const Vertex& v0 = *face->V0();
			const Vertex& v1 = *face->V1();
			const Vertex& v2 = *face->V2();

			VR::Vec4 edge1(v0.Pos(), v1.Pos());
			assert(edge1.Length() > 0.0); // no degenerated edge ?
			VR::Vec4 edge2(v0.Pos(), v2.Pos());
			assert(edge2.Length() > 0.0); // no degenerated edge ?

			VR::Vec4 normal = VR::Vec4::CrossProduct(edge1, edge2);
			normal = VR::Vec4::Normalize(normal);

			float dPlaneComponent = normal.x * vertex.Pos().x;
			dPlaneComponent += normal.y * vertex.Pos().y;
			dPlaneComponent += normal.z * vertex.Pos().z;
			dPlaneComponent *= -1.f;
			VR::Vec4 newPlane(normal.x, normal.y, normal.z, dPlaneComponent);
			planes.push_back(newPlane);
		}

		// sum the fundamental error quadric for each plane
		Quadric* quadric = new Quadric();
		for (size_t curPlane = 0; curPlane < planes.size(); ++curPlane)
		{
			const VR::Vec4& p = planes[curPlane];
			float vals[] = {p.x*p.x, p.x*p.y, p.x*p.z, p.x*p.w,
							p.y*p.y, p.y*p.z, p.y*p.w, 
							p.z*p.z, p.z*p.w,
							p.w*p.w};
			quadric->Add(vals);
		}
		vertex.SetAssociatedQuadric(quadric);
	}

	printf("[+] Initial quadrics computed\n");
}
// ----------------------------------------------------------------------------
void Mesh::SelectAndComputeVertexPairs()
{
	assert(pairs_.size() == 0);

	printf("[ ] Selecting and computing valid pairs\n");

	std::set<PairType>::const_iterator it = edges_.begin();
	std::set<PairType>::const_iterator end = edges_.end();
	for (; it != end; ++it)
	{
		VertexPair* newPair = new VertexPair(it->first, it->second);
		newPair->ComputePosAndQuadric();
		newPair->ComputeQuadricError();
#if 0
		printf("Pair(%d, %d) -> ERROR = \t\t\t\t%f\n", newPair->V0()->Id(), newPair->V1()->Id(), newPair->QuadricError());
#endif
		assert(newPair->QuadricError() >= 0.f);
		pairs_.push_back(newPair);

		newPair->V0()->AddPair(newPair);
		newPair->V1()->AddPair(newPair);
	}

	printf("[+] Valid pairs selected and computed\n");

	assert(pairs_.size() >= edges_.size()); // there is at least all edges
}
// ---------------------------------------------------------------------------
void Mesh::Simplify()
{
#if 1
	size_t nbContractions = 3000; // FIXME: define REAL criteria
#else
	size_t nbContractions = 1000; // FIXME: define REAL criteria
#endif

	printf("[ ] Simplifying mesh...\n");

	printf("\t nbPairs: %d\n", pairs_.size());

	while (!pairs_.empty() && (nbContractions > 0))
	{
		VertexPair* pair = ExtractCostlessVertexPair_();
		assert(pair);
		//assert(pair->QuadricError() >= 0.f);

		pair->Contract();
		printf("nbContractLeft: %d\n", nbContractions);
		nbContractions--;

		delete pair;
	}

	printf("[+] Mesh Simplified\n");
}
// ---------------------------------------------------------------------------
VertexPair* Mesh::ExtractCostlessVertexPair_()
{
	assert(pairs_.size() > 0);

	float minCost = std::numeric_limits<float>::max();
	VertexPair* minPair = 0;

	for (size_t curPair = 0; curPair < pairs_.size(); ++curPair)
	{
		VertexPair* pair = pairs_[curPair];

#if 0
		if (!pair->DeleteMe())
			printf("Pair(%d, %d) -> ERROR = \t%f\n", pair->V0()->Id(), pair->V1()->Id(), pair->QuadricError());
#endif

		if (!pair->DeleteMe() && (pair->QuadricError() < minCost))
		{
			minCost = pair->QuadricError();
			minPair = pair;
		}
	}

	pairs_.erase(std::find(pairs_.begin(), pairs_.end(), minPair));

	printf("error: %f\n", minCost);

	assert(minPair);
	return minPair;
}
// ---------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================