#ifndef UPDATABLE_HEAP_HPP_
#define UPDATABLE_HEAP_HPP_

#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
template<typename T>
UpdatableHeap<T>::UpdatableHeap()
{
}
// ----------------------------------------------------------------------------
template<typename T>
UpdatableHeap<T>::~UpdatableHeap()
{
}
// ----------------------------------------------------------------------------
template<typename T>
void UpdatableHeap<T>::Insert(T parValue)
{
	tree_.push_back(parValue);

	size_t curInd = tree_.size() - 1;
	while (curInd > 0)
	{
		size_t parent = (curInd - 1) / 2;

		if (tree_[parent] > parValue)
			Swap_(parent, curInd);
		else
			return;

		curInd = parent;
	}
}
// ----------------------------------------------------------------------------
template<typename T>
T UpdatableHeap<T>::ExtractMin()
{
	assert(tree_.size() > 0);

	T result = tree_[0];
	Swap_(0, tree_.size() - 1);
	tree_.pop_back();
	ReorderHeapFromTopToBottom_(0);

	return result;
}
// ----------------------------------------------------------------------------
// WARNING: The type T may need to reimplement the '<' operator !!!
template<typename T>
void UpdatableHeap<T>::ReorderHeapFromTopToBottom_(size_t parInd)
{
	size_t size = tree_.size();
	assert(parInd < size);

	size_t min = parInd;
	size_t left = 2 * parInd + 1;
	size_t right = 2 * parInd + 2;

	if ((left < size) && (tree_[left] < tree_[min]))
		min = left;
	if ((right < size) && (tree_[right] < tree_[min]))
		min = right;

	if (min != parInd)
	{
		Swap_(parInd, min);
		ReorderHeapFromTopToBottom_(min);
	}
}
// ----------------------------------------------------------------------------
template<typename T>
bool UpdatableHeap<T>::IsValid(size_t parInd) const
{
	size_t size = tree_.size();

	assert(parInd < size);

	size_t left = 2 * parInd + 1;
	size_t right = 2 * parInd + 2;

	if ((left < size) &&
		((tree_[left] < tree_[parInd]) || !IsValid(left)))
	{
		return false;
	}
	if ((right < size) &&
		((tree_[right] < tree_[parInd]) || !IsValid(right)))
	{
		return false;
	}

	return true;
}
// ----------------------------------------------------------------------------
template<typename T>
void UpdatableHeap<T>::Swap_(size_t parIndA, size_t parIndB)
{
	assert(parIndA < tree_.size());
	assert(parIndB < tree_.size());

	T tmp = tree_[parIndA];
	tree_[parIndA] = tree_[parIndB];
	tree_[parIndB] = tmp;
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif