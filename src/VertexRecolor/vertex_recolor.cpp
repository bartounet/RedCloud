
#include "vertex_recolor.h"
#include "mesh_file_helper.h"
#include "threedtree.h"
#include <assert.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
namespace VR
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
void VertexRecolor(Mesh& parFinalMesh, const Mesh& parColoredMesh, const Mesh& parTriMesh)
{
	assert(parFinalMesh.vertices.size() == 0);
	assert(parFinalMesh.faces.size() == 0);
	assert(parColoredMesh.vertices.size() > 0);
	assert(parColoredMesh.faces.size() == 0);
	assert(parTriMesh.vertices.size() > 0);
	assert(parTriMesh.faces.size() > 0);

	MeshFileHelper::DeepCopyMesh(parFinalMesh, parTriMesh);

	printf("\t[ ] Building 3d-Tree...\n");

	std::vector<const Vertex*> verticesPtr;
	for (size_t curVertex = 0; curVertex < parColoredMesh.vertices.size(); ++curVertex)
		verticesPtr.push_back(&parColoredMesh.vertices[curVertex]);

	ThreeDNode* tree = static_cast<ThreeDNode*>( ThreeDNode::BuildTree(verticesPtr, 0) );

	printf("\t[+] 3D-Tree build\n");

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
		const Vertex* nearestVertex = &parColoredMesh.vertices[0];
		float minDist = 10000000.f;
		ThreeDNode::NearestNeighbor(tree, dstVertex, 0, minDist, &nearestVertex);
		dstVertex.r = nearestVertex->r;
		dstVertex.g = nearestVertex->g;
		dstVertex.b = nearestVertex->b;
	}

	delete tree;
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
// ----------------------------------------------------------------------------