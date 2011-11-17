
#include "mesh.h"
#include "vertex.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <set>
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
	for (size_t curVertex = 0; curVertex < nbVertices; ++curVertex)
	{
#if 0
		printf("%d\n", curVertex);
#endif
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex], curVertex));
	}
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
		Quadric* quadric = new Quadric();
		for (size_t curPlane = 0; curPlane < planes.size(); ++curPlane)
		{
			const VR::Vec4& p = planes[curPlane];
			double vals[] = {p.x*p.x, p.x*p.y, p.x*p.z, p.x*p.w,
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
#ifdef OPTIMIZE
	assert(pairsHeap_.Size() == 0);
#else
	assert(pairs_.size() == 0);
#endif

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
		assert(newPair->QuadricError() >= 0.0);
#ifdef OPTIMIZE
		pairsHeap_.Insert(newPair);
#else
		pairs_.push_back(newPair);
#endif

		newPair->V0()->AddPair(newPair);
		newPair->V1()->AddPair(newPair);
	}

	printf("[+] Valid pairs selected and computed\n");

#ifdef OPTIMIZE
	assert(pairsHeap_.Size() >= edges_.size());
#else
	assert(pairs_.size() >= edges_.size()); // there is at least all edges
#endif
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

#ifdef OPTIMIZE
	printf("\t nbPairs: %d\n", pairsHeap_.Size());
#else
	printf("\t nbPairs: %d\n", pairs_.size());
#endif

	// FIXME: Attention, si on contract un bord, on ne perd qu'une face !!
	size_t nbContractions = (faces_.size() - parMaxFaces) / 2; // a chaque contraction on supprime 2 faces (OU PLUS !)
#ifdef OPTIMIZE
	while (!pairsHeap_.Empty() && (nbContractions > 0))
#else
	while (!pairs_.empty() && (nbContractions > 0))
#endif
	{
		VertexPair* pair = 0;

#ifdef OPTIMIZE
		pair = pairsHeap_.ExtractMin();
#else
		pair = ExtractCostlessVertexPair_();
#endif

		assert(pair);
		assert(!pair->DeleteMe());
		assert(pair->QuadricError() >= 0.0);

#if 0
		printf("nbContractLeft: %d\n", nbContractions);
		//printf("contracting: %d -> %d\n", pair->V0()->Id(), pair->V1()->Id());
#endif

#ifdef OPTIMIZE
		std::vector<VertexPair*> deletePairs;
		std::vector<VertexPair*> updatePairs;
		pair->Contract(deletePairs, updatePairs);

		deletePairs.erase(std::find(deletePairs.begin(), deletePairs.end(), pair)); // ignore the current pair
		pairsHeap_.Delete(deletePairs);
		pairsHeap_.Update(updatePairs);
#else
		pair->Contract(); // ici, on sait quelles paires vont etre mises a jours et celles qui sont modifs
#endif

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
// ---------------------------------------------------------------------------
#ifndef OPTIMIZE
VertexPair* Mesh::ExtractCostlessVertexPair_()
{
	assert(pairs_.size() > 0);

	double minCost = std::numeric_limits<double>::max();
	VertexPair* minPair = 0;

	for (size_t curPair = 0; curPair < pairs_.size(); ++curPair)
	{
		VertexPair* pair = pairs_[curPair];

		if (!pair->DeleteMe() && (pair->QuadricError() < minCost))
		{
			minCost = pair->QuadricError();
			minPair = pair;
		}
	}

	assert(minPair);
	pairs_.erase(std::find(pairs_.begin(), pairs_.end(), minPair));
	
	return minPair;
}
#endif
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
