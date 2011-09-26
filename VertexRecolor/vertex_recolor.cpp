
#include "vertex_recolor.h"
#include "mesh_file_helper.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
// At the moment, done by an exhaustive research in O(N)
static const Vertex& FindNearestVertex(const Mesh& parMesh, const Vertex& parTarget)
{
	int nbVertices = parMesh.vertices.size();
	assert(nbVertices > 0);

	float minDist = 1000000.f;
	const Vertex* nearest = &parMesh.vertices[0];

	for (int curVertex = 0; curVertex < nbVertices; ++curVertex)
	{
		const Vertex& v = parMesh.vertices[curVertex];
		float dist = (parTarget.x - v.x) * (parTarget.x - v.x);
		dist += (parTarget.y - v.y) * (parTarget.y - v.y);
		dist += (parTarget.z - v.z) * (parTarget.z - v.z);

		if (dist < minDist)
		{
			minDist = dist;
			nearest = &v;
		}
	}

	return *nearest;
}
// ----------------------------------------------------------------------------
bool VertexRecolor(Mesh& parFinalMesh, const Mesh& parColoredMesh, const Mesh& parTriMesh)
{
	assert(parFinalMesh.vertices.size() == 0);
	assert(parFinalMesh.faces.size() == 0);
	assert(parColoredMesh.vertices.size() > 0);
	assert(parColoredMesh.faces.size() == 0);
	assert(parTriMesh.vertices.size() > 0);
	assert(parTriMesh.faces.size() > 0);

	MeshFileHelper::DeepCopyMesh(parFinalMesh, parTriMesh);

	int nbDstVertices = parFinalMesh.vertices.size();
	int curDecimalPercent = 0;
	const int tenthPercent = nbDstVertices / 10;
	for (int curDstVertex = 0; curDstVertex < nbDstVertices; ++curDstVertex)
	{
		if (curDstVertex >= (curDecimalPercent * tenthPercent))
		{
			printf("\t%d percent done (%d on %d).\n", curDecimalPercent * 10, curDstVertex, nbDstVertices);
			curDecimalPercent++;
		}

		Vertex& dstVertex = parFinalMesh.vertices[curDstVertex];
		const Vertex& nearestVertex = FindNearestVertex(parColoredMesh, dstVertex);
		dstVertex.r = nearestVertex.r;
		dstVertex.g = nearestVertex.g;
		dstVertex.b = nearestVertex.b;
	}

	return true;
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================