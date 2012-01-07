
#include "mesh.h"
#include "vertex_pair.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <limits>
#include "../common/threedtree.h"
#include <map>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Mesh::Mesh(const Com::Mesh& parVRMesh)
{
	printf("- Constructing Mesh from VRMesh\n");

	uint nbVertices = parVRMesh.vertices.size();
	assert(nbVertices > 0);
	assert(nbVertices < BAD_VERTEX_ID); // if it fails, grow up the BAD_VERTEX_ID value
	printf("\t- Copying %d Vertices\n", nbVertices);

	vertices_.reserve(nbVertices);
	for (uint curVertex = 0; curVertex < nbVertices; ++curVertex)
		vertices_.push_back(new Vertex(parVRMesh.vertices[curVertex], curVertex));
	assert(vertices_.size() == nbVertices);

	uint nbFaces = parVRMesh.faces.size();
	assert(nbFaces > 0);
	printf("\t- Copying %d Faces\n", nbFaces);
	for (uint curFace = 0; curFace < nbFaces; ++curFace)
	{
		Vertex* v0 = vertices_[parVRMesh.faces[curFace].vertices[0]];
		Vertex* v1 = vertices_[parVRMesh.faces[curFace].vertices[1]];
		Vertex* v2 = vertices_[parVRMesh.faces[curFace].vertices[2]];
		faces_.push_back(new Face(v0, v1, v2));
	}

	printf("\t- Generating adjacency\n");
	GenerateAdjacency_();
}
// ----------------------------------------------------------------------------
Mesh::~Mesh()
{
}
// ----------------------------------------------------------------------------
void Mesh::GenerateAdjacency_()
{
	assert(vertices_.size() > 0);
	assert(faces_.size() > 0);

	// incident face
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		Face* face = faces_[curFace];

		face->V0()->AddIncidentFace(face);
		face->V1()->AddIncidentFace(face);
		face->V2()->AddIncidentFace(face);
	}
}
// ----------------------------------------------------------------------------
void Mesh::MergeCloseVertices_(	const std::vector<double>& parNNDists,
								const std::vector<Vertex*> parNNs,
								double parMergeDist)
{
	assert(parNNDists.size() == vertices_.size());
	assert(parNNs.size() == vertices_.size());
	assert(parNNDists.size() == parNNs.size());

	typedef std::map<Vertex*,Vertex*> MapType;
	typedef MapType::iterator MapItType;
	MapType mergeDstVertices;
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		if (parNNDists[curVertex] > parMergeDist) // FIXME: Use variance
			continue;

		Vertex* v0 = vertices_[curVertex];
		assert(v0);
		while (v0->DeleteMe())
		{
			MapItType it = mergeDstVertices.find(v0);
			assert(it != mergeDstVertices.end());
			v0 = it->second;
		}
		assert(!v0->DeleteMe());

		Vertex* v1 = parNNs[curVertex];
		assert(v1);
		while (v1->DeleteMe())
		{
			MapItType it = mergeDstVertices.find(v1);
			assert(it != mergeDstVertices.end());
			v1 = it->second;
		}
		assert(!v1->DeleteMe());

		if (v0 == v1)
			continue;

		v1->ReplaceThisInIncidentFacesWith(v0);
		v1->RemoveDegeneratedFaces();
		v0->AddIncidentFaces(v1->IncidentFaces());
		v0->RemoveDegeneratedFaces();

		v1->SetDeleteMe();
		mergeDstVertices.insert(std::make_pair(v1, v0));
	}
}
// ----------------------------------------------------------------------------
void Mesh::MarkIsolatedVerticesAndFacesToDelete_(	const std::vector<double>& parNNDists,
													double parIsolateDist)
{
	assert(parNNDists.size() == vertices_.size());

	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		if (parNNDists[curVertex] < parIsolateDist)
			continue;

		Vertex* v0 = vertices_[curVertex];
		v0->SetDeleteMe();
		v0->SetDeleteMeOnRelatedFaces();
	}
}
// ----------------------------------------------------------------------------
void Mesh::MarkDegeneratedFacesToDelete_()
{
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		Face& face = *faces_[curFace];
		assert(face.DeleteMe() || !face.IsDegenerated()); // vertices merge fail

		if (face.HasZeroAreaSurface())
			face.SetDeleteMe();
	}
}
// ----------------------------------------------------------------------------
void Mesh::Clean()
{
	printf("\t- Building 3d-tree\n");
	std::vector<const Vertex*> verticesPtr;
	for (size_t curVertex = 0; curVertex < vertices_.size(); ++curVertex)
		verticesPtr.push_back(vertices_[curVertex]);
	Com::ThreeDNode<Vertex>* tree = static_cast<Com::ThreeDNode<Vertex>*>( Com::ThreeDNode<Vertex>::BuildTree(verticesPtr, 0, 1000) );
	assert(tree);

	printf("\t- Searching nearest neighbours\n");
	double meanAccum = 0.0;
	std::vector<double> distances;
	std::vector<Vertex*> nearestVertices;
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& targetVertex = *vertices_[curVertex];
		assert(!targetVertex.DeleteMe());
		double dist = std::numeric_limits<double>::max();
		const Vertex* nearestVertex = 0;
		Com::ThreeDNode<Vertex>::NearestNeighbour(tree, targetVertex, 0, dist, &nearestVertex);
		assert(nearestVertex);

		distances.push_back(dist);
		nearestVertices.push_back(const_cast<Vertex*>(nearestVertex));

		meanAccum += dist;
	}
	double meanDist = meanAccum / double(vertices_.size());
	assert(meanDist >= 0.0);

	printf("\t- Merging closes vertices\n");
	MergeCloseVertices_(distances, nearestVertices, 0.03 * meanDist); // FIXME: Tweak dist value

	printf("\t- Remove isolated vertices and faces\n");
	MarkIsolatedVerticesAndFacesToDelete_(distances, 20.0 * meanDist); // FIXME: Tweak dist value

	printf("\t- Removing degenerated faces\n");
	MarkDegeneratedFacesToDelete_();

	DeleteFacesIFN();

	SetDeleteUnusedVerticesAndReassignVerticesId();
	DeleteVerticesIFN();

//#ifdef _DEBUG
#if 1
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
		assert( !faces_[curFace]->DeleteMe() &&
				!faces_[curFace]->IsDegenerated() &&
				!faces_[curFace]->HasZeroAreaSurface());
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
		assert(!vertices_[curVertex]->DeleteMe());
#endif
}
// ----------------------------------------------------------------------------
struct VertexDeleteCmp
{
	bool operator()(Vertex* parA, Vertex* parB)
	{
		if (parA->DeleteMe() == parB->DeleteMe())
			return parA < parB; // strict ordering
		return !(parA->DeleteMe());
	}
};
void Mesh::DeleteVerticesIFN()
{
	typedef std::vector<Vertex*>::iterator VertexItType;

	if (vertices_.size() == 0)
		return;

	std::sort(vertices_.begin(), vertices_.end(), VertexDeleteCmp());

	assert(vertices_.size() > 0);
	VertexItType vertexIt = --(vertices_.end());
	assert(!vertices_.front()->DeleteMe());
	while (vertexIt != vertices_.begin())
	{
		Vertex* vertex = *vertexIt;

		if (vertex->DeleteMe())
		{
			// FIXME: Remove related faces
			delete vertex;
		}
		else
			break;

		--vertexIt;
	}
	vertexIt++;

	vertices_.erase(vertexIt, vertices_.end());

#ifdef _DEBUG
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
		assert(!vertices_[curVertex]->DeleteMe());
#endif
}
// ----------------------------------------------------------------------------
struct FaceDeleteCmp
{
	bool operator()(Face* parA, Face* parB)
	{
		if (parA->DeleteMe() == parB->DeleteMe())
			return parA < parB;
		return !(parA->DeleteMe());
	}
};
void Mesh::DeleteFacesIFN()
{
	typedef std::vector<Face*>::iterator FaceItType;

	if (faces_.size() == 0)
		return;

	std::sort(faces_.begin(), faces_.end(), FaceDeleteCmp());

	assert(faces_.size() > 0);
	FaceItType faceIt = --(faces_.end());
	while (faceIt != faces_.begin())
	{
		Face* face = *faceIt;

		if (face->DeleteMe())
		{
			face->V0()->RemoveIncidentFaceIFN(face);
			face->V1()->RemoveIncidentFaceIFN(face);
			face->V2()->RemoveIncidentFaceIFN(face);
			delete face;
		}
		else
			break;
		--faceIt;
	}
	faceIt++;

	faces_.erase(faceIt, faces_.end());

#ifdef _DEBUG
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
		assert(!faces_[curFace]->DeleteMe());
#endif
}
// ----------------------------------------------------------------------------
void Mesh::SetDeleteUnusedVerticesAndReassignVerticesId()
{
	printf("- Marking unreferenced vertices and reassigning Vertex Id...\n");

	std::set<Vertex*> usedVertices;
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face& face = *faces_[curFace];
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
		Vertex& vertex = *vertices_[curVertex];

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
}
// ----------------------------------------------------------------------------
void Mesh::ExportToVRMesh(Com::Mesh& parDstMesh) const
{
	assert(parDstMesh.faces.size() == 0);
	assert(parDstMesh.vertices.size() == 0);

	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face& face = *faces_[curFace];
		if (!face.IsDegenerated())
		{
			assert(!face.DeleteMe());
			parDstMesh.faces.push_back(Com::Face(face.V0()->Id(), 
												face.V1()->Id(),
												face.V2()->Id()));
		}
	}

	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		const Vertex& vertex = *vertices_[curVertex];
		if (!vertex.DeleteMe())
			parDstMesh.vertices.push_back(Com::Vertex(vertex.Pos()));
	}
}
// ----------------------------------------------------------------------------
void Mesh::ComputeNormals(std::vector<Com::Vec4>& parNormals)
{
	assert(parNormals.size() == 0);
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& vertex = *vertices_[curVertex];
		//assert(!vertex.DeleteMe());
		if (vertex.DeleteMe())
			continue;
		Com::Vec4 normal = Com::Vec4(0, 0, 0, 1);

		// for each incident faces, compute the plane
		for (uint curFace = 0; curFace < vertex.IncidentFaces().size(); ++curFace)
		{
			const Face* face = vertex.IncidentFaces()[curFace];
			//assert(!face->DeleteMe());
			//assert(!face->HasZeroAreaSurface());
			if (face->DeleteMe() || face->HasZeroAreaSurface())
				continue;

			const Vertex& v0 = *face->V0();
			const Vertex& v1 = *face->V1();
			const Vertex& v2 = *face->V2();

			Com::Vec4 edge1(v0.Pos(), v1.Pos());
			//assert(edge1.Length() > 0.0); // no degenerated edge ?
			if (edge1.Length() == 0.0)
				continue;
			Com::Vec4 edge2(v0.Pos(), v2.Pos());
			//assert(edge2.Length() > 0.0); // no degenerated edge ?
			if (edge2.Length() == 0.0)
				continue;

			Com::Vec4 normalFace = Com::Vec4::CrossProduct(edge1, edge2);
			double length = normalFace.Length();
			if (length == 0.0f)
				continue;
			normalFace = Com::Vec4::Normalize(normalFace);
			normal.x += normalFace.x;
			normal.y += normalFace.y;
			normal.z += normalFace.z;
			normal.w += normalFace.w;
		}
		double length = normal.Length();
		if (length == 0.0f)
			continue;
		normal = Com::Vec4::Normalize(normal);
		parNormals.push_back(normal);
	}
}
// ----------------------------------------------------------------------------
void Mesh::ComputeInitialQuadrics()
{
	printf("- Computing initial quadrics\n");

#ifdef _DEBUG
	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
		assert(!vertices_[curVertex]->DeleteMe());
#endif

	for (uint curVertex = 0; curVertex < vertices_.size(); ++curVertex)
	{
		Vertex& vertex = *vertices_[curVertex];
		assert(!vertex.DeleteMe());
		std::vector<Com::Vec4> planes;

		// for each incident faces, compute the plane
		for (uint curFace = 0; curFace < vertex.IncidentFaces().size(); ++curFace)
		{
			const Face* face = vertex.IncidentFaces()[curFace];
			assert(!face->DeleteMe());
			assert(!face->HasZeroAreaSurface());

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
}
// ----------------------------------------------------------------------------
void Mesh::SelectAndComputeVertexPairs()
{
	assert(pairsHeap_.Size() == 0);

	printf("- Selecting and computing valid pairs\n");

	std::set<PairType> edges;
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
	{
		const Face* face = faces_[curFace];
		edges.insert(PairType(std::min(face->V0(), face->V1()), std::max(face->V0(), face->V1())));
		edges.insert(PairType(std::min(face->V0(), face->V2()), std::max(face->V0(), face->V2())));
		edges.insert(PairType(std::min(face->V1(), face->V2()), std::max(face->V1(), face->V2())));
	}

	// FIXME: Add non-edge vertex pair

	std::set<PairType>::const_iterator it = edges.begin();
	std::set<PairType>::const_iterator end = edges.end();
	for (; it != end; ++it)
	{
		VertexPair* newPair = new VertexPair(it->first, it->second);
		newPair->ComputePosAndQuadric();
		newPair->AssignQuadricErrorWithNewValue();
		assert(newPair->QuadricError() >= 0.0);

		pairsHeap_.Insert(newPair);

		newPair->V0()->AddPair(newPair);
		newPair->V1()->AddPair(newPair);
	}
	assert(pairsHeap_.Size() >= edges.size()); // there is at least all edges
}
// ---------------------------------------------------------------------------
uint Mesh::NbValidFaces_() const
{
	uint nbValidFaces = 0;

	std::vector<Face*>::const_iterator it = faces_.begin();
	std::vector<Face*>::const_iterator end = faces_.end();
	for (; it != end; ++it)
		if (!(*it)->IsDegenerated())
			nbValidFaces++;

	return nbValidFaces;
}
// ---------------------------------------------------------------------------
uint Mesh::NbValidFaces() const
{
	return NbValidFaces_();
}
// ---------------------------------------------------------------------------
std::vector<Face*>& Mesh::Faces()
{
	return faces_;
}
// ---------------------------------------------------------------------------
bool Mesh::HasZeroAreaSurfaceFaces() const
{
	for (uint curFace = 0; curFace < faces_.size(); ++curFace)
		if (faces_[curFace]->HasZeroAreaSurface())
			return true;
	return false;
}
// ---------------------------------------------------------------------------
void Mesh::Simplify(uint parMaxFaces)
{
	assert(parMaxFaces > 0); // the simpliest mesh will be a triangle
	assert(!HasZeroAreaSurfaceFaces());
	assert(faces_.size() > parMaxFaces);

	printf("- Simplifying mesh (pairs count : %d)...\n", pairsHeap_.Size());

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

	SetDeleteUnusedVerticesAndReassignVerticesId();
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
