#ifndef THREE_D_TREE_H_
#define THREE_D_TREE_H_


#include "geometry.h"
#include <vector>


namespace Com
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
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
template <typename VertexType>
class ThreeDNode : public Node
{
public:
	ThreeDNode(const VertexType* parVertex, AlignedAxisDir parDir);
	virtual ~ThreeDNode();

public:
	static Node* BuildTree(const std::vector<const VertexType*>& parVertices, int parDepth, const uint parMaxVertexPerLeaf);
	static void NearestPoint(const Node* parNode,
							 const Vec4& parTargetPos,
							 int parDepth,
							 double& parMinDist,
							 const VertexType** parMinVertex);
	static void NearestNeighbour(	const Node* parTree,
									const VertexType& parTarget,
									int parDepth,
									double& parMinDist,
									const VertexType** parMinVertex);

private:
	AlignedAxisDir dir_;
	const VertexType* vertex_;
	Node* left_; // lesser and equal
	Node* right_; // greater
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
template <typename VertexType>
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<const VertexType*> parVertices);
	virtual ~LeafNode();

	const std::vector<const VertexType*>& Vertices() const { return vertices_; }

private:
	std::vector<const VertexType*> vertices_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}

#include "threedtree.impl.hpp"


#endif