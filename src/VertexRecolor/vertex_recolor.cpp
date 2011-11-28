
#include "vertex_recolor.h"
#include "../common/mesh_file_helper.h"
#include "../common/threedtree.h"
#include <assert.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
namespace VR
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
void VertexRecolor(Com::Mesh&		parFinalMesh,
				   const Com::Mesh& parColoredMesh,
				   const Com::Mesh& parTriMesh)
{
	assert(parFinalMesh.vertices.size() == 0);
	assert(parFinalMesh.faces.size() == 0);
	assert(parColoredMesh.vertices.size() > 0);
	assert(parColoredMesh.faces.size() == 0);
	assert(parTriMesh.vertices.size() > 0);
	assert(parTriMesh.faces.size() > 0);

	Com::MeshFileHelper::DeepCopyMesh(parFinalMesh, parTriMesh);

	printf("\t[ ] Building 3d-Tree...\n");

	std::vector<const Com::Vertex*> verticesPtr;
	for (size_t curVertex = 0; curVertex < parColoredMesh.vertices.size(); ++curVertex)
		verticesPtr.push_back(&parColoredMesh.vertices[curVertex]);

	Com::ThreeDNode<Com::Vertex>* tree = 
		static_cast<Com::ThreeDNode<Com::Vertex>*>( Com::ThreeDNode<Com::Vertex>::BuildTree(verticesPtr, 0, 1000) );

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

		Com::Vertex& dstVertex = parFinalMesh.vertices[curDstVertex];
		const Com::Vertex* nearestVertex = &parColoredMesh.vertices[0];
		double minDist = 10000000.f;
		Com::ThreeDNode<Com::Vertex>::NearestPoint(tree, dstVertex, 0, minDist, &nearestVertex);
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