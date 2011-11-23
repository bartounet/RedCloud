#ifndef THREE_D_TREE_H_
#define THREE_D_TREE_H_


#include "geometry.h"
#include <vector>


#define MAX_VERTEX_PER_LEAF 1000


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace Com
{
// ----------------------------------------------------------------------------
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
	ThreeDNode(const Vertex* parVertex, AlignedAxisDir parDir);
	virtual ~ThreeDNode();

public:
	static Node* BuildTree(const std::vector<const Vertex*>& parVertices, int parDepth);
	static void NearestPoint(const Node* parNode,
							 const Vertex& parTarget,
							 int parDepth,
							 float& minDist,
							 const Vertex** minVertex);

private:
	AlignedAxisDir dir_;
	const Vertex* vertex_;
	Node* left_; // lesser and equal
	Node* right_; // greater
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<const Vertex*> parVertices);
	virtual ~LeafNode();

	const std::vector<const Vertex*>& Vertices() const { return vertices_; }

private:
	std::vector<const Vertex*> vertices_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif