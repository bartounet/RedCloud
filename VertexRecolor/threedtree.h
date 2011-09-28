#ifndef THREE_D_TREE_H_
#define THREE_D_TREE_H_


#include "geometry.h"
#include <vector>


#define MAX_VERTEX_PER_LEAF 1000


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
enum AlignedAxisDir
{
	AADIR_X,
	AADIR_Y,
	AADIR_Z
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Node
{
public:
	Node() {}
	virtual ~Node() {}
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class ThreeDNode : public Node
{
public:
	ThreeDNode(const Vertex& parVertex, AlignedAxisDir parDir);
	virtual ~ThreeDNode();

public:
	static Node* BuildTree(const std::vector<Vertex>& parVertices, int parDepth);
	static void NearestNeighbor(const Node* parNode,
								const Vertex& parTarget,
								int parDepth,
								float& minDist,
								Vertex& minVertex);

private:
	AlignedAxisDir dir_;
	const Vertex& vertex_;
	Node* left_; // lesser and equal
	Node* right_; // greater
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<Vertex> parVertices);
	virtual ~LeafNode();

	const std::vector<Vertex>& Vertices() const { return vertices_; }

private:
	std::vector<Vertex> vertices_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif