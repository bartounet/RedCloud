
#include "threedtree.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
ThreeDNode::ThreeDNode(const Vertex& parVertex, AlignedAxisDir parDir) :
	Node(),
	dir_(parDir),
	left_(0),
	right_(0),
	vertex_(parVertex)
{
}
// ----------------------------------------------------------------------------
ThreeDNode::~ThreeDNode()
{
	if (left_)
		delete left_;
	if (right_)
		delete right_;
}
// ----------------------------------------------------------------------------
static const uint MAX_PLANE_DIR = 3;
static AlignedAxisDir planeDir[] = {AADIR_X, AADIR_Y, AADIR_Z};
// ----------------------------------------------------------------------------
Node* ThreeDNode::BuildTree(const std::vector<Vertex>& parVertices, int parDepth)
{
	if (parVertices.size() <= MAX_VERTEX_PER_LEAF)
		return new LeafNode(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	const Vertex& median = parVertices[0]; // FIXME

	std::vector<Vertex> left;
	std::vector<Vertex> right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		const Vertex& vertex = parVertices[curVertex];

		switch (dir)
		{
		case AADIR_X:
			if (vertex.x <= median.x)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Y:
			if (vertex.y <= median.y)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Z: 
			if (vertex.z <= median.z)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		default:
			assert(false); // it should never happen
			break;
		};
	}

	ThreeDNode* node = new ThreeDNode(median, dir);
	node->left_ = BuildTree(left, parDepth + 1);
	node->right_ = BuildTree(right, parDepth + 1);

	return node;
}
// ----------------------------------------------------------------------------
#define SQ(x) ((x) * (x))
void ThreeDNode::NearestNeighbor(	const Node* parNode,
									const Vertex& parTarget,
									int parDepth,
									float& minDist,
									Vertex& minVertex)
{
	const LeafNode* leafNode = dynamic_cast<const LeafNode*>(parNode);
	if (leafNode)
	{
		const std::vector<Vertex>& vertices = leafNode->Vertices();
		const Vertex& nearestVertex = vertices[0];
		for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		{
			const Vertex& v = vertices[curVertex];
			float dist = SQ(parTarget.x - v.x) + SQ(parTarget.y - v.y) + SQ(parTarget.z - v.z);
			if (dist < minDist)
			{
				minDist = dist;
				minVertex = v;
			}
		}
	}
	else
	{
		const ThreeDNode* threeDNode = static_cast<const ThreeDNode*>(parNode);
		float dist = SQ(parTarget.x - threeDNode->vertex_.x);
		dist += SQ(parTarget.y - threeDNode->vertex_.y);
		dist += SQ(parTarget.z - threeDNode->vertex_.z);
		if (dist < minDist)
		{
			minDist = dist;
			minVertex = threeDNode->vertex_;
		}

		AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
		bool goToLeftNode = false;
		switch (dir)
		{
		case AADIR_X:
			if (parTarget.x <= threeDNode->vertex_.x)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		case AADIR_Y:
			if (parTarget.y <= threeDNode->vertex_.y)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		case AADIR_Z:
			if (parTarget.y <= threeDNode->vertex_.y)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		default:
			assert(false); // it should never happen
			break;
		}
	}
}
#undef SQ
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
LeafNode::LeafNode(const std::vector<Vertex> parVertices) :
	Node(),
	vertices_(parVertices)
{
}
// ----------------------------------------------------------------------------
LeafNode::~LeafNode()
{
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
