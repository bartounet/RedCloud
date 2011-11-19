
#include "vertex_pair_heap.h"
#include "vertex_pair.h"
#include <assert.h>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
VertexPairHeap::VertexPairHeap()
{
}
// ----------------------------------------------------------------------------
VertexPairHeap::~VertexPairHeap()
{
	for (uint ind = 0; ind < tree_.size(); ++ind)
		delete tree_[ind];
}
// ----------------------------------------------------------------------------
void VertexPairHeap::Insert(VertexPair* parVertexPair)
{
	assert(parVertexPair);

	tree_.push_back(parVertexPair);
	parVertexPair->SetHeapInd(tree_.size() - 1);

	uint curInd = tree_.size() - 1;
	while (curInd > 0)
	{
		uint parent = (curInd - 1) / 2;

		if (tree_[parent]->QuadricError() > parVertexPair->QuadricError())
			Swap_(parent, curInd);
		else
			return;

		curInd = parent;
	}
}
// ----------------------------------------------------------------------------
VertexPair* VertexPairHeap::ExtractMin()
{
	assert(tree_.size() > 0);

	VertexPair* result = tree_[0];

	Swap_(0, tree_.size() - 1);
	tree_.pop_back();
	DownHeap_(0);

	return result;
}
// ----------------------------------------------------------------------------
void VertexPairHeap::Delete(const std::vector<VertexPair*>& parDeletePairs)
{
	for (uint curPair = 0; curPair < parDeletePairs.size(); ++curPair)
	{
		VertexPair* pair = parDeletePairs[curPair];
		assert(pair->DeleteMe());

		uint pairInd = pair->HeapInd();

		if (pairInd == (tree_.size() - 1)) // deleting the last element
			tree_.pop_back();
		else
		{
			Swap_(pairInd, tree_.size() - 1);
			tree_.pop_back();
			pairInd = DownHeap_(pairInd);
			UpHeap_(pairInd);
		}
	}

#ifdef _DEBUG
	assert(IsValid());
#endif
}
// ----------------------------------------------------------------------------
void VertexPairHeap::Update(const std::vector<VertexPair*>& parUpdatePairs)
{
	for (uint curPair = 0; curPair < parUpdatePairs.size(); ++curPair)
	{
		VertexPair* pair = parUpdatePairs[curPair];
		assert(!pair->DeleteMe());

		pair->AssignQuadricErrorWithNewValue(); // FIXME: Can we avoid this ?
		
		uint pairInd = pair->HeapInd();
		pairInd = DownHeap_(pairInd);
		UpHeap_(pairInd);
	}

#ifdef _DEBUG
	assert(IsValid());
#endif
}
// ----------------------------------------------------------------------------
#ifdef _DEBUG
bool VertexPairHeap::IsValid(uint parInd) const
{
	uint size = tree_.size();
	assert(parInd < size);

	uint left = 2 * parInd + 1;
	uint right = 2 * parInd + 2;

	if ((left < size) &&
		((tree_[left]->QuadricError() < tree_[parInd]->QuadricError()) || 
		!IsValid(left)))
	{
		return false;
	}
	if ((right < size) &&
		((tree_[right]->QuadricError() < tree_[parInd]->QuadricError()) ||
		!IsValid(right)))
	{
		return false;
	}

	return true;
}
#endif
// ----------------------------------------------------------------------------
#ifdef _DEBUG
uint VertexPairHeap::LinearFindInTree(const VertexPair* parPair) const
{
	assert(parPair);

	for (uint curPair = 0; curPair < tree_.size(); ++curPair)
		if (tree_[curPair] == parPair)
			return curPair;

	assert(false);
	return -1;
}
#endif
// ----------------------------------------------------------------------------
void VertexPairHeap::Swap_(uint parIndA, uint parIndB)
{
	assert(parIndA < tree_.size());
	assert(parIndB < tree_.size());

	VertexPair* tmp = tree_[parIndA];
	tree_[parIndA] = tree_[parIndB];
	tree_[parIndB] = tmp;

	uint tmpInd = tree_[parIndA]->HeapInd();
	tree_[parIndA]->SetHeapInd(tree_[parIndB]->HeapInd());
	tree_[parIndB]->SetHeapInd(tmpInd);
}
// ----------------------------------------------------------------------------
uint VertexPairHeap::DownHeap_(uint parInd)
{
	uint size = tree_.size();
	assert(parInd < size);

	uint curInd = parInd;
	while (1)
	{
		uint left = 2 * curInd + 1;
		uint right = 2 * curInd + 2;

		uint min = curInd;
		if ((left < size) && (tree_[left]->QuadricError() < tree_[min]->QuadricError()))
			min = left;
		if ((right < size) && (tree_[right]->QuadricError() < tree_[min]->QuadricError()))
			min = right;

		if (min != curInd)
		{
			Swap_(curInd, min);
			curInd = min;
		}
		else
			break;
	}

	return curInd;
}
// ----------------------------------------------------------------------------
uint VertexPairHeap::UpHeap_(uint parInd)
{
	uint size = tree_.size();
	assert(parInd < size);

	uint curInd = parInd;
	while (curInd > 0)
	{
		uint parent = (curInd - 1) / 2;

		if (tree_[curInd]->QuadricError() < tree_[parent]->QuadricError())
		{
			Swap_(curInd, parent);
			curInd = parent;
		}
		else
			break;
	}

	return curInd;
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
