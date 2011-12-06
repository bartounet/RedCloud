
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
	if (parVertices.size() <= parMaxVertexPerLeaf)
		return new LeafNode<VertexType>(parVertices);

	AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
	const VertexType* pivot = parVertices[rand() % parVertices.size()];

	std::vector<const VertexType*> left;
	std::vector<const VertexType*> right;
	for (size_t curVertex = 0; curVertex < parVertices.size(); ++curVertex)
	{
		const VertexType* vertex = parVertices[curVertex];

		#define LOCAL_COMPONENT_TEST_AND_PUSH(parComponent)					\
			if (vertex->Pos().parComponent <= pivot->Pos().parComponent)	\
				left.push_back(vertex);										\
			else															\
				right.push_back(vertex);									\
			break;

		switch (dir)
		{
		case AADIR_X: LOCAL_COMPONENT_TEST_AND_PUSH(x)
		case AADIR_Y: LOCAL_COMPONENT_TEST_AND_PUSH(y)
		case AADIR_Z: LOCAL_COMPONENT_TEST_AND_PUSH(z)
		default: assert(false); break; // it should never happen
		}

		#undef LOCAL_COMPONENT_TEST_AND_PUSH
	}

	ThreeDNode* node = new ThreeDNode(pivot, dir);
	node->left_ = BuildTree(left, parDepth + 1, parMaxVertexPerLeaf);
	node->right_ = BuildTree(right, parDepth + 1, parMaxVertexPerLeaf);

	return node;
}
// ----------------------------------------------------------------------------
#define SQ(x) ((x) * (x))
template <typename VertexType>
void ThreeDNode<VertexType>::NearestPoint(	const Node* parNode,
											const Vec4& parTargetPos,
											int parDepth,
											double& parMinDist,
											const VertexType** parMinVertex)
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
			double dist = SQ(parTargetPos.x - v->Pos().x) + SQ(parTargetPos.y - v->Pos().y) + SQ(parTargetPos.z - v->Pos().z);
			if (dist < parMinDist)
			{
				parMinDist = dist;
				*parMinVertex = v;
			}
		}
	}
	else
	{
		const ThreeDNode* threeDNode = static_cast<const ThreeDNode*>(parNode);
		double dist = SQ(parTargetPos.x - threeDNode->vertex_->Pos().x);
		dist += SQ(parTargetPos.y - threeDNode->vertex_->Pos().y);
		dist += SQ(parTargetPos.z - threeDNode->vertex_->Pos().z);
		if (dist < parMinDist)
		{
			parMinDist = dist;
			*parMinVertex = threeDNode->vertex_;
		}

		AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
		const Node* nextNode = 0;

		#define LOCAL_COMPONENT_TEST(parCompo)										\
			if (parTargetPos.parCompo <= threeDNode->vertex_->Pos().parCompo)	\
				nextNode = threeDNode->left_;										\
			else																	\
				nextNode = threeDNode->right_;										\
			break;

		switch (dir)
		{
		case AADIR_X: LOCAL_COMPONENT_TEST(x)
		case AADIR_Y: LOCAL_COMPONENT_TEST(y)
		case AADIR_Z: LOCAL_COMPONENT_TEST(z)
		default: assert(false); break; // it should never happen
		}
		assert(nextNode);

		#undef LOCAL_COMPONENT_TEST

		NearestPoint(nextNode, parTargetPos, parDepth + 1, parMinDist, parMinVertex);
	}
}
#undef SQ
// ----------------------------------------------------------------------------
#define SQ(x) ((x) * (x))
template <typename VertexType>
void ThreeDNode<VertexType>::NearestNeighbour(	const Node* parTree,
												const VertexType& parTarget,
												int parDepth,
												double& parMinDist,
												const VertexType** parMinVertex)
{
	const LeafNode<VertexType>* leafNode = dynamic_cast<const LeafNode<VertexType>*>(parTree);
	if (leafNode)
	{
		const std::vector<const VertexType*>& vertices = leafNode->Vertices();
		if (vertices.size() <= 0)
			return;
		for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		{
			const VertexType* v = vertices[curVertex];
			double dist = SQ(parTarget.Pos().x - v->Pos().x)
				+ SQ(parTarget.Pos().y - v->Pos().y)
				+ SQ(parTarget.Pos().z - v->Pos().z);
			if ((dist < parMinDist) && (v != &parTarget))
			{
				parMinDist = dist;
				*parMinVertex = v;
			}
		}
	}
	else
	{
		const ThreeDNode* threeDNode = static_cast<const ThreeDNode*>(parTree);
		double dist = SQ(parTarget.Pos().x - threeDNode->vertex_->Pos().x);
		dist += SQ(parTarget.Pos().y - threeDNode->vertex_->Pos().y);
		dist += SQ(parTarget.Pos().z - threeDNode->vertex_->Pos().z);
		if ((dist < parMinDist) && (threeDNode->vertex_ != &parTarget))
		{
			parMinDist = dist;
			*parMinVertex = threeDNode->vertex_;
		}

		AlignedAxisDir dir = planeDir[parDepth % MAX_PLANE_DIR];
		const Node* nextNode = 0;
		
		#define LOCAL_COMPONENT_TEST(parCompo)										\
			if (parTarget.Pos().parCompo <= threeDNode->vertex_->Pos().parCompo)	\
				nextNode = threeDNode->left_;										\
			else																	\
				nextNode = threeDNode->right_;										\
			break;

		switch (dir)
		{
		case AADIR_X: LOCAL_COMPONENT_TEST(x)
		case AADIR_Y: LOCAL_COMPONENT_TEST(y)
		case AADIR_Z: LOCAL_COMPONENT_TEST(z)
		default: assert(false); break; // it should never happen
		}

		#undef LOCAL_COMPONENT_TEST

		NearestNeighbour(nextNode, parTarget, parDepth + 1, parMinDist, parMinVertex);
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
