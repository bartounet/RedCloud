#ifndef UPDATABLE_HEAP_H_
#define UPDATABLE_HEAP_H_


#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
template<typename T>
class UpdatableHeap
{
public:
	UpdatableHeap();
	~UpdatableHeap();

public:
	void Insert(T parValue);
	T ExtractMin();

// debug only
public:
	bool IsValid(size_t parInd = 0) const;
// debug only

private:
	void ReorderHeapFromTopToBottom_(size_t parInd);
	void Swap_(size_t parIndA, size_t parIndB);

private: 
	std::vector<T> tree_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#include "updatable_heap.hpp"

#endif