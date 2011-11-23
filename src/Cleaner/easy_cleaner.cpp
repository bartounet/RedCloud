
#include "easy_cleaner.h"
#include "../common/threedtree.h"
#include <assert.h>
#include <stdio.h>
#include <set>

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace EC
{
/*
**	Build a Tree for a given Mesh
*/
Com::ThreeDNode *getThreeDTree(Com::Mesh& parMesh)
{
	std::vector<std::pair<const Com::Vertex*, size_t> > verticesPtr;
	for (size_t curVertex = 0; curVertex < parMesh.vertices.size(); ++curVertex)
		verticesPtr.push_back(std::make_pair(&parMesh.vertices[curVertex], curVertex));

	return static_cast<Com::ThreeDNode*>( Com::ThreeDNode::BuildTree(verticesPtr, 0) );
}

/*
**	Get the mean distance for a given Mesh and Tree, and fill a vector of distances
*/
double getMeanDistance(Com::Mesh& parMesh, Com::ThreeDNode* parTree,
					   std::vector<std::pair<double, size_t> >& distancesToFill)
{
	assert(distancesToFill.size() == 0);
	double meanDist = 0;
	for (size_t curVertex = 0; curVertex < parMesh.vertices.size(); ++curVertex)
	{
		Com::Vertex& dstVertex = parMesh.vertices[curVertex];
		size_t nearestVertexIndex = 0;
		float minDist = 10000000.f;
		Com::ThreeDNode::NearestNeighbor(parTree, dstVertex, 0, minDist, &nearestVertexIndex);
		meanDist += minDist;
		distancesToFill.push_back(std::make_pair(minDist, nearestVertexIndex));
	}
	meanDist /= parMesh.vertices.size();
	return meanDist;
}

/*
**	Apply the selection criteria for vertices and then get faces to remove
*/
void faceAndVertexIndexesToRemove(Com::Mesh& parMesh, Com::ThreeDNode* parTree, std::vector<size_t>& parVIndexesToFill,
								  std::set<size_t>& parFIndexesToFill)
{
	printf("\t\t[ ] Computing Mean Distance\n");
	std::vector<std::pair<double, size_t> > distances;
	double meanDistance = getMeanDistance(parMesh, parTree, distances);
	printf("\t\t[+] Mean Distance computed : %f\n", meanDistance);
	printf("\t\t[+] Limit Distance computed : %f\n", meanDistance / 20000);
	printf("\t\t[ ] Vertexes Selection\n");
	size_t endIndex = 1;
	size_t count = 0;
	for (size_t curVertex = 0; curVertex <= distances.size() - endIndex; ++curVertex)
	{
		// Isolated point
		if (distances[curVertex].first > 10 * meanDistance)
		{
			// We put the vertices at the end of the vector because it simplifies the face deletion
			// Once we've swapped the vertices and computed the associated face, we restart on the same
			// index
			std::vector<size_t> facesToFill;
			size_t newIndex = distances.size() - endIndex;
			
			parMesh.swapVertex(curVertex, newIndex);
			parMesh.updateFacesToBeDeleted(curVertex, newIndex, facesToFill);
			std::swap(distances[curVertex].first, distances[newIndex].first);
			std::swap(distances[curVertex].second, distances[newIndex].second);
			parVIndexesToFill.push_back(newIndex);
			for (size_t curFace = 0; curFace < facesToFill.size(); ++curFace)
				parFIndexesToFill.insert(facesToFill[curFace]);

			endIndex++;
			curVertex--;
		}
		// Too close points
		/*else if (distances[curVertex].first < (meanDistance / 800000)) 
		{
			count++;
			if (count % 100 == 0)
				printf("count : %d\n", count);
			// Merge
			std::vector<size_t> facesToFill;
			size_t mergeIndex = distances[curVertex].second;
			parMesh.mergeVerticesInFaces(curVertex, mergeIndex, facesToFill);
			// Update distances index
			for (size_t i = curVertex; i <= distances.size() - endIndex; i++)
				if (distances[i].second == curVertex)
					distances[i].second = mergeIndex;
			// nearest_neighbour(A) = B   =/=>  nearest_neighbour(B) = A. So 
			// if it is the case, we don't calculate the second nearest point
			// we just consider that the other point won't be merged
			if (distances[mergeIndex].second == curVertex)
				distances[mergeIndex].first = meanDistance;
			
			// Swap
			size_t newIndex = distances.size() - endIndex;
			parMesh.swapVertex(curVertex, newIndex);
			parMesh.updateFacesToBeDeleted(curVertex, newIndex, facesToFill);
			facesToFill.clear();
			std::swap(distances[curVertex].first, distances[newIndex].first);
			std::swap(distances[curVertex].second, distances[newIndex].second);
			for (size_t i = 0; i <= distances.size() - endIndex; i++)
				if (distances[i].second == newIndex)
					distances[i].second = curVertex;
			parVIndexesToFill.push_back(newIndex);
			for (size_t curFace = 0; curFace < facesToFill.size(); ++curFace)
				parFIndexesToFill.insert(facesToFill[curFace]);		

			endIndex++;
			curVertex--;
		}*/
	}
	printf("\t\t[+] Vertexes selected\n");

}


void deleteFacesAndVertex(Com::Mesh& parMesh, std::vector<size_t>& parVIndexes,
						  std::set<size_t>& parFIndexes)
{
// We remove the 'end_index' last vertices
	//parMesh.vertices.erase(parMesh.vertices.begin() + parVIndexes.back(),
	//					   parMesh.vertices.begin() + parVIndexes.front());
// A set sorts the elements
	std::set<size_t>::iterator it = parFIndexes.begin();
	for (size_t i = 0; it != parFIndexes.end(); ++i, ++it)
	{
		size_t index = *it - i;
		parMesh.faces.erase(parMesh.faces.begin() + index);
	}
}


// ----------------------------------------------------------------------------
void EasyClean(Com::Mesh& parMesh)
{
	assert(parMesh.vertices.size() != 0);
	printf("\t[ ] Building 3d-Tree...\n");
	Com::ThreeDNode* tree = getThreeDTree(parMesh);
	printf("\t[+] 3D-Tree build\n");
// Get indexes of vertices and faces to remove
	printf("\t[ ] Get indexes to remove\n");
	std::vector<size_t> VIndexesToRemove;
	std::set<size_t> FIndexesToRemove; // Set because we don't want to store the same index more than once
	faceAndVertexIndexesToRemove(parMesh, tree, VIndexesToRemove, FIndexesToRemove);
	printf("\t[+] Size of list of vertexes to remove : %d\n", VIndexesToRemove.size());
	printf("\t[ ] Deletion...\n");
	deleteFacesAndVertex(parMesh, VIndexesToRemove, FIndexesToRemove);
	printf("\t[+] Deleted\n");
	delete tree;

}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================