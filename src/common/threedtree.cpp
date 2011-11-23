
#include "threedtree.h"
#include <assert.h>
#include <stdlib.h>


// ----------------------------------------------------------------------------
namespace Com
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
#ifdef JO_FIXME
ThreeDNode::ThreeDNode(std::pair<const Vertex*, size_t> parVertex, AlignedAxisDir parDir) :
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
Node* ThreeDNode::BuildTree(const std::vector<std::pair<const Vertex*, size_t> >& parVertices,
							int parDepth)
{
	if (parVertices.size() <= MAX_VERTEX_PER_LEAF)
		return new LeafNode(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	size_t pivotIndex = rand() % parVertices.size();
	const Vertex* pivot = parVertices[pivotIndex].first;
	std::pair<const Vertex*, size_t> pivotPair = std::make_pair(pivot, pivotIndex);

	std::vector<std::pair<const Vertex*, size_t> > left;
	std::vector<std::pair<const Vertex*, size_t> > right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		std::pair<const Vertex*, size_t> vertexPair = parVertices[curVertex];

		switch (dir)
		{
		case AADIR_X:
			if (vertexPair.first->x <= pivot->x)
				left.push_back(vertexPair);
			else
				right.push_back(vertexPair);
			break;
		case AADIR_Y:
			if (vertexPair.first->y <= pivot->y)
				left.push_back(vertexPair);
			else
				right.push_back(vertexPair);
			break;
		case AADIR_Z: 
			if (vertexPair.first->z <= pivot->z)
				left.push_back(vertexPair);
			else
				right.push_back(vertexPair);
			break;
		default:
			assert(false); // it should never happen
			break;
		};
	}

	ThreeDNode* node = new ThreeDNode(pivotPair, dir);
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
									size_t* minVertexIndex)
{
	const LeafNode* leafNode = dynamic_cast<const LeafNode*>(parNode);
	if (leafNode)
	{
		const std::vector<std::pair<const Vertex*, size_t> >& vertices = leafNode->Vertices();
		if (vertices.size() <= 0)
			return;
		const Vertex* nearestVertex = vertices[0].first;
		for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		{
			const Vertex* v = vertices[curVertex].first;
			float dist = SQ(parTarget.x - v->x) + SQ(parTarget.y - v->y) + SQ(parTarget.z - v->z);
			if (&parTarget != v && dist < minDist)
			{
				minDist = dist;
				*minVertexIndex = vertices[curVertex].second;
			}
		}
	}
	else
	{
		const ThreeDNode* threeDNode = static_cast<const ThreeDNode*>(parNode);
		float dist = SQ(parTarget.x - threeDNode->vertex_.first->x);
		dist += SQ(parTarget.y - threeDNode->vertex_.first->y);
		dist += SQ(parTarget.z - threeDNode->vertex_.first->z);
		if (&parTarget != threeDNode->vertex_.first && dist < minDist)
		{
			minDist = dist;
			*minVertexIndex = threeDNode->vertex_.second;
		}

		AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
		bool goToLeftNode = false;
		switch (dir)
		{
		case AADIR_X:
			if (parTarget.x <= threeDNode->vertex_.first->x)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertexIndex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertexIndex);
			break;
		case AADIR_Y:
			if (parTarget.y <= threeDNode->vertex_.first->y)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertexIndex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertexIndex);
			break;
		case AADIR_Z:
			if (parTarget.z <= threeDNode->vertex_.first->z)
				NearestNeighbor(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertexIndex);
			else
				NearestNeighbor(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertexIndex);
			break;
		default:
			assert(false); // it should never happen
			break;
		}
	}
}
#undef SQ
#else
ThreeDNode::ThreeDNode(const Vertex* parVertex, AlignedAxisDir parDir) :
	Node(),
	dir_(parDir),
	vertex_(parVertex),
	left_(0),
	right_(0)
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
Node* ThreeDNode::BuildTree(const std::vector<const Vertex*>& parVertices, int parDepth)
{
	if (parVertices.size() <= MAX_VERTEX_PER_LEAF)
		return new LeafNode(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	const Vertex* pivot = parVertices[rand() % parVertices.size()];

	std::vector<const Vertex*> left;
	std::vector<const Vertex*> right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		const Vertex* vertex = parVertices[curVertex];

		switch (dir)
		{
		case AADIR_X:
			if (vertex->x <= pivot->x)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Y:
			if (vertex->y <= pivot->y)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Z: 
			if (vertex->z <= pivot->z)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		default:
			assert(false); // it should never happen
			break;
		};
	}

	ThreeDNode* node = new ThreeDNode(pivot, dir);
	node->left_ = BuildTree(left, parDepth + 1);
	node->right_ = BuildTree(right, parDepth + 1);

	return node;
}
// ----------------------------------------------------------------------------
#define SQ(x) ((x) * (x))
void ThreeDNode::NearestPoint(		const Node* parNode,
									const Vertex& parTarget,
									int parDepth,
									float& minDist,
									const Vertex** minVertex)
{
	const LeafNode* leafNode = dynamic_cast<const LeafNode*>(parNode);
	if (leafNode)
	{
		const std::vector<const Vertex*>& vertices = leafNode->Vertices();
		if (vertices.size() <= 0)
			return;
		for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		{
			const Vertex* v = vertices[curVertex];
			float dist = SQ(parTarget.x - v->x) + SQ(parTarget.y - v->y) + SQ(parTarget.z - v->z);
			if (dist < minDist)
			{
				minDist = dist;
				*minVertex = v;
			}
		}
	}
	else
	{
		const ThreeDNode* threeDNode = static_cast<const ThreeDNode*>(parNode);
		float dist = SQ(parTarget.x - threeDNode->vertex_->x);
		dist += SQ(parTarget.y - threeDNode->vertex_->y);
		dist += SQ(parTarget.z - threeDNode->vertex_->z);
		if (dist < minDist)
		{
			minDist = dist;
			*minVertex = threeDNode->vertex_;
		}

		AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
		switch (dir)
		{
		case AADIR_X:
			if (parTarget.x <= threeDNode->vertex_->x)
				NearestPoint(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestPoint(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		case AADIR_Y:
			if (parTarget.y <= threeDNode->vertex_->y)
				NearestPoint(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestPoint(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		case AADIR_Z:
			if (parTarget.z <= threeDNode->vertex_->z)
				NearestPoint(threeDNode->left_, parTarget, parDepth + 1, minDist, minVertex);
			else
				NearestPoint(threeDNode->right_, parTarget, parDepth + 1, minDist, minVertex);
			break;
		default:
			assert(false); // it should never happen
			break;
		}
	}
}
#undef SQ
#endif
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
#ifdef JO_FIXME
LeafNode::LeafNode(const std::vector<std::pair<const Vertex*, size_t> > parVertices) :
	Node(),
	vertices_(parVertices)
{
}
// ----------------------------------------------------------------------------
LeafNode::~LeafNode()
{
}
#else
LeafNode::LeafNode(const std::vector<const Vertex*> parVertices) :
	Node(),
	vertices_(parVertices)
{
}
// ----------------------------------------------------------------------------
LeafNode::~LeafNode()
{
}
#endif
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
// ----------------------------------------------------------------------------
