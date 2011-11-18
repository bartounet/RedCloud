
#include "mesh.h"
#include "vertex_pair.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <limits>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Mesh::Mesh(const VR::Mesh& parVRMesh)
{
	printf("[ ] Constructing Mesh from VRMesh\n");

	size_t nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);
	printf("\t[ ] Copying %d Vertices\n", nbVertices);

	vertices_.reserve(nbVertices);
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex], curVertex));
	assert(vertices_.size() == nbVertices);
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

			double dPlaneComponent = normal.x * vertex.Pos().x;
			dPlaneComponent += normal.y * vertex.Pos().y;
			dPlaneComponent += normal.z * vertex.Pos().z;
			dPlaneComponent *= -1.0;
			VR::Vec4 newPlane(normal.x, normal.y, normal.z, dPlaneComponent);
			planes.push_back(newPlane);
		}

		// sum the fundamental error quadric for each plane
		Quadric quadric;
		for (size_t curPlane = 0; curPlane < planes.size(); ++curPlane)
		{
			const VR::Vec4& p = planes[curPlane];
			double vals[] = {p.x*p.x, p.x*p.y, p.x*p.z, p.x*p.w,
							p.y*p.y, p.y*p.z, p.y*p.w, 
							p.z*p.z, p.z*p.w,
							p.w*p.w};
			quadric.Add(vals);
		}
		vertex.SetAssociatedQuadric(quadric);
	}

	printf("[+] Initial quadrics computed\n");
}
// ----------------------------------------------------------------------------
void Mesh::SelectAndComputeVertexPairs()
{
	assert(pairsHeap_.Size() == 0);

	printf("[ ] Selecting and computing valid pairs\n");

	std::set<PairType>::const_iterator it = edges_.begin();
	std::set<PairType>::const_iterator end = edges_.end();
	for (; it != end; ++it)
	{
		VertexPair* newPair = new VertexPair(it->first, it->second);
		newPair->ComputePosAndQuadric();
		assert(newPair->QuadricError() >= 0.0);

		pairsHeap_.Insert(newPair);

		newPair->V0()->AddPair(newPair);
		newPair->V1()->AddPair(newPair);
	}

	printf("[+] Valid pairs selected and computed\n");

	assert(pairsHeap_.Size() >= edges_.size()); // there is at least all edges
}
// ---------------------------------------------------------------------------
size_t Mesh::NbValidFaces_() const
{
	size_t nbValidFaces = 0;

	std::vector<Face>::const_iterator it = faces_.begin();
	std::vector<Face>::const_iterator end = faces_.end();
	for (; it != end; ++it)
		if (!it->IsDegenerated())
			nbValidFaces++;

	return nbValidFaces;
}
// ---------------------------------------------------------------------------
void Mesh::Simplify(size_t parMaxFaces)
{
	assert(parMaxFaces > 0); // the simpliest mesh will be a triangle
	assert(faces_.size() > parMaxFaces);

	printf("[ ] Simplifying mesh...\n");
	printf("\t nbPairs: %d\n", pairsHeap_.Size());

	// FIXME: Attention, si on contract un bord, on ne perd qu'une face !!
	size_t nbContractions = (faces_.size() - parMaxFaces) / 2; // a chaque contraction on supprime 2 faces (OU PLUS !)
	while (!pairsHeap_.Empty() && (nbContractions > 0))
	{
		VertexPair* pair = pairsHeap_.ExtractMin();

		assert(pair);
		assert(!pair->DeleteMe());
		assert(pair->QuadricError() >= 0.0);

#if 0
		printf("nbContractLeft: %d\n", nbContractions);
		//printf("contracting: %d -> %d\n", pair->V0()->Id(), pair->V1()->Id());
#endif

		std::vector<VertexPair*> deletePairs;
		std::vector<VertexPair*> updatePairs;

		pair->Contract(deletePairs, updatePairs);

		deletePairs.erase(std::find(deletePairs.begin(), deletePairs.end(), pair)); // ignore the current pair
		pairsHeap_.Delete(deletePairs);
		pairsHeap_.Update(updatePairs);

#if 0
		if ((nbContractions & 127) == 0)
			printf("\tfaces: %d\n", NbValidFaces_());
#endif

		nbContractions--;
		assert(pair);
		delete pair;

		// === DELETE ME ===
		// FIXME: Hack pour tomber en dessous du nombre de face necessaire. On 
		// devrait calculer le nombre de contraction necessaire en amont !!
		if ((nbContractions <= 0) && (NbValidFaces_() > parMaxFaces))
			nbContractions = 1;
		// === DELETE ME ===
	}

	assert(NbValidFaces_() <= parMaxFaces);

	printf("[+] Mesh Simplified\n");
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
