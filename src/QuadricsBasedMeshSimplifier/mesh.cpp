
#include "mesh.h"
#include "vertex_pair.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <limits>
#include "../common/threedtree.h"


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Mesh::Mesh(const Com::Mesh& parVRMesh)
{
	printf("[ ] Constructing Mesh from VRMesh\n");

	uint nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);
	assert(nbVertices < BAD_VERTEX_ID); // if it fails, grow up the BAD_VERTEX_ID value
	printf("\t[ ] Copying %d Vertices\n", nbVertices);

	vertices_.reserve(nbVertices);
	for (uint curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(Vertex(parVRMesh.vertices[curVertex], curVertex));
	assert(vertices_.size() == nbVertices);
	printf("\t[+] Vertices copied\n");

	uint nbFaces = parVRMesh.faces.size();
	assert(nbFaces > 0);
	printf("\t[ ] Copying %d Faces\n", nbFaces);
	for (uint curFace = 0; curFace < nbFaces; ++curFace)
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
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
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
void Mesh::Clean()
{
	std::vector<const Vertex*> verticesPtr;
	for (size_t curVertex = 0; curVertex < vertices_.size(); ++curVertex)
		verticesPtr.push_back(&vertices_[curVertex]);
	Com::ThreeDNode<Vertex>* tree = static_cast<Com::ThreeDNode<Vertex>*>( Com::ThreeDNode<Vertex>::BuildTree(verticesPtr, 0, 1000) );
	assert(tree);

	// Calculer la distance moyenne (au carre) entre chaque vertex
		// Pour chaque vertex
		//   Trouver son plus proche voisin
		//   calculer la distance avec ce voisin
	// Pour chaque vertex
	//  Si sa distance avec son plus proche voisin est inferieur a 1% de la moyenne
	//  Merger les vertices
}
// ----------------------------------------------------------------------------
void Mesh::ReassignVerticesIdAndSetDeleteUnusedVertices_()
{
	printf("[ ] Reassigning Vertex Id...\n");

	std::set<Vertex*> usedVertices;
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face& face = faces_[curFace];
		if (!face.IsDegenerated())
		{
			usedVertices.insert(face.V0());
			usedVertices.insert(face.V1());
			usedVertices.insert(face.V2());
		}
	}

	uint index = 0;
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& vertex = vertices_[curVertex];

		if (vertex.DeleteMe())
			vertex.SetId(BAD_VERTEX_ID);
		else
		{
			if (usedVertices.find(&vertex) == usedVertices.end())
			{
				vertex.SetDeleteMe();
				vertex.SetId(BAD_VERTEX_ID);
			}
			else
			{
				vertex.SetId(index);
				index++;
			}
		}
	}

	printf("[+] Vertex Id reassigned...\n");
}
// ----------------------------------------------------------------------------
void Mesh::ExportToVRMesh(Com::Mesh& parDstMesh) const
{
	assert(parDstMesh.faces.size() == 0);
	assert(parDstMesh.vertices.size() == 0);

	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face& face = faces_[curFace];
		if (!face.IsDegenerated())
		{
			parDstMesh.faces.push_back(Com::Face(face.V0()->Id(), 
												face.V1()->Id(),
												face.V2()->Id()));
		}
	}

	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		const Vertex& vertex = vertices_[curVertex];
		if (!vertex.DeleteMe())
			parDstMesh.vertices.push_back(Com::Vertex(vertex.Pos()));
	}
}
// ----------------------------------------------------------------------------
void Mesh::ComputeInitialQuadrics()
{
	printf("[ ] Computing initial quadrics\n");

	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& vertex = vertices_[curVertex];
		std::vector<Com::Vec4> planes;

		// for each incident faces, compute the plane
		for (uint curFace = 0; curFace < vertex.IncidentFaces().size(); ++curFace)
		{
			const Face* face = vertex.IncidentFaces()[curFace];

			const Vertex& v0 = *face->V0();
			const Vertex& v1 = *face->V1();
			const Vertex& v2 = *face->V2();

			Com::Vec4 edge1(v0.Pos(), v1.Pos());
			assert(edge1.Length() > 0.0); // no degenerated edge ?
			Com::Vec4 edge2(v0.Pos(), v2.Pos());
			assert(edge2.Length() > 0.0); // no degenerated edge ?

			Com::Vec4 normal = Com::Vec4::CrossProduct(edge1, edge2);
			normal = Com::Vec4::Normalize(normal);

			double dPlaneComponent = normal.x * vertex.Pos().x;
			dPlaneComponent += normal.y * vertex.Pos().y;
			dPlaneComponent += normal.z * vertex.Pos().z;
			dPlaneComponent *= -1.0;
			Com::Vec4 newPlane(normal.x, normal.y, normal.z, dPlaneComponent);
			planes.push_back(newPlane);
		}

		// sum the fundamental error quadric for each plane
		Quadric quadric;
		for (uint curPlane = 0; curPlane < planes.size(); ++curPlane)
		{
			const Com::Vec4& p = planes[curPlane];
			double vals[] = {p.x*p.x, p.x*p.y, p.x*p.z, p.x*p.w,
							p.y*p.y, p.y*p.z, p.y*p.w, 
							p.z*p.z, p.z*p.w,
							p.w*p.w};
			quadric.Add(vals);
		}
		vertex.SetQuadric(quadric);
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
uint Mesh::NbValidFaces_() const
{
	uint nbValidFaces = 0;

	std::vector<Face>::const_iterator it = faces_.begin();
	std::vector<Face>::const_iterator end = faces_.end();
	for (; it != end; ++it)
		if (!it->IsDegenerated())
			nbValidFaces++;

	return nbValidFaces;
}
// ---------------------------------------------------------------------------
bool Mesh::HasZeroAreaSurfaceFaces() const
{
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
		if (faces_[curFace].HasZeroAreaSurface())
			return true;
	return false;
}
// ---------------------------------------------------------------------------
void Mesh::Simplify(uint parMaxFaces)
{
	assert(parMaxFaces > 0); // the simpliest mesh will be a triangle
	assert(!HasZeroAreaSurfaceFaces());
	assert(faces_.size() > parMaxFaces);

	printf("[ ] Simplifying mesh...\n");
	printf("\t nbPairs: %d\n", pairsHeap_.Size());

	uint curNbFaces = faces_.size();
	while (!pairsHeap_.Empty() && (curNbFaces > parMaxFaces))
	{
		VertexPair* pair = pairsHeap_.ExtractMin();
		assert(pair);
		assert(!pair->DeleteMe());
		assert(!pair->IsDegenerated());
		assert(pair->QuadricError() >= 0.0);
		assert(!pair->V0()->DeleteMe());
		assert(!pair->V1()->DeleteMe());

#if 0
		if ((curNbFaces & 1023) == 0)
			printf("curNbFaces: %d\n", curNbFaces);
		//printf("contracting: %d -> %d\n", pair->V0()->Id(), pair->V1()->Id());
#endif

		std::vector<VertexPair*> deletePairs;
		std::vector<VertexPair*> updatePairs;

		uint nbFaceDeleted = pair->Contract(deletePairs, updatePairs);
		curNbFaces -= nbFaceDeleted;

		pairsHeap_.Delete(deletePairs);
		pairsHeap_.Update(updatePairs);

		assert(pair);
		delete pair;
	}

	assert(NbValidFaces_() <= parMaxFaces);
	//assert(!HasZeroAreaSurfaceFaces()); // FIXME: Need post-clean

	ReassignVerticesIdAndSetDeleteUnusedVertices_();

	printf("[+] Mesh Simplified\n");
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
