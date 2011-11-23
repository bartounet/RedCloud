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
#ifdef JO_FIXME
class ThreeDNode : public Node
{
public:
	ThreeDNode(std::pair<const Vertex*, size_t> parVertex, AlignedAxisDir parDir);
	virtual ~ThreeDNode();

public:
	static Node* BuildTree(const std::vector<std::pair<const Vertex*, size_t> >& parVertices, int parDepth);
	static void NearestNeighbor(const Node* parNode,
								const Vertex& parTarget,
								int parDepth,
								float& minDist,
								size_t* minVertexIndex);

private:
	AlignedAxisDir dir_;
	std::pair<const Vertex*, size_t> vertex_;
	Node* left_; // lesser and equal
	Node* right_; // greater
};
#else
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
#endif
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
#ifdef JO_FIXME
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<std::pair<const Vertex*, size_t> > parVertices);
	virtual ~LeafNode();

	const std::vector<std::pair<const Vertex*, size_t> >& Vertices() const { return vertices_; }

private:
	std::vector<std::pair<const Vertex*, size_t> > vertices_;
};
#else
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<const Vertex*> parVertices);
	virtual ~LeafNode();

	const std::vector<const Vertex*>& Vertices() const { return vertices_; }

private:
	std::vector<const Vertex*> vertices_;
};
#endif
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif