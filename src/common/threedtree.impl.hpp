
#include <assert.h>
#include <stdlib.h>
#include "../QuadricsBasedMeshSimplifier/vertex.h" // needed for buildtree specialization


// ----------------------------------------------------------------------------
namespace Com
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
template <typename VertexType>
ThreeDNode<VertexType>::ThreeDNode(const VertexType* parVertex, AlignedAxisDir parDir) :
	Node(),
	dir_(parDir),
	vertex_(parVertex),
	left_(0),
	right_(0)
{
}
// ----------------------------------------------------------------------------
template <typename VertexType>
ThreeDNode<VertexType>::~ThreeDNode()
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
template <typename VertexType>
Node* ThreeDNode<VertexType>::BuildTree(const std::vector<const VertexType*>& parVertices, int parDepth, const uint parMaxVertexPerLeaf)
{
	assert(false);
	return 0;
}

template <>
Node* ThreeDNode<QBMS::Vertex>::BuildTree(const std::vector<const QBMS::Vertex*>& parVertices, int parDepth, const uint parMaxVertexPerLeaf)
{
	if (parVertices.size() <= parMaxVertexPerLeaf)
		return new LeafNode<QBMS::Vertex>(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	const QBMS::Vertex* pivot = parVertices[rand() % parVertices.size()];

	std::vector<const QBMS::Vertex*> left;
	std::vector<const QBMS::Vertex*> right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		const QBMS::Vertex* vertex = parVertices[curVertex];

		switch (dir)
		{
		case AADIR_X:
			if (vertex->Pos().x <= pivot->Pos().x)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Y:
			if (vertex->Pos().y <= pivot->Pos().y)
				left.push_back(vertex);
			else
				right.push_back(vertex);
			break;
		case AADIR_Z: 
			if (vertex->Pos().z <= pivot->Pos().z)
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
	node->left_ = BuildTree(left, parDepth + 1, parMaxVertexPerLeaf);
	node->right_ = BuildTree(right, parDepth + 1, parMaxVertexPerLeaf);

	return node;

}

template <>
Node* ThreeDNode<Com::Vertex>::BuildTree(const std::vector<const Com::Vertex*>& parVertices, int parDepth, const uint parMaxVertexPerLeaf)
{
	if (parVertices.size() <= parMaxVertexPerLeaf)
		return new LeafNode<Com::Vertex>(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	const Com::Vertex* pivot = parVertices[rand() % parVertices.size()];

	std::vector<const Com::Vertex*> left;
	std::vector<const Com::Vertex*> right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		const Com::Vertex* vertex = parVertices[curVertex];

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
	node->left_ = BuildTree(left, parDepth + 1, parMaxVertexPerLeaf);
	node->right_ = BuildTree(right, parDepth + 1, parMaxVertexPerLeaf);

	return node;
}
// ----------------------------------------------------------------------------
#define SQ(x) ((x) * (x))
template <typename VertexType>
void ThreeDNode<VertexType>::NearestPoint(		const Node* parNode,
									const VertexType& parTarget,
									int parDepth,
									float& minDist,
									const VertexType** minVertex)
{
	const LeafNode<VertexType>* leafNode = dynamic_cast<const LeafNode<VertexType>*>(parNode);
	if (leafNode)
	{
		const std::vector<const VertexType*>& vertices = leafNode->Vertices();
		if (vertices.size() <= 0)
			return;
		for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		{
			const VertexType* v = vertices[curVertex];
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
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
template <typename VertexType>
LeafNode<VertexType>::LeafNode(const std::vector<const VertexType*> parVertices) :
	Node(),
	vertices_(parVertices)
{
}
// ----------------------------------------------------------------------------
template <typename VertexType>
LeafNode<VertexType>::~LeafNode()
{
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
// ----------------------------------------------------------------------------
