#ifndef VERTEX_PAIR_HEAP_H_

#if 0
#ifndef OPTIMIZE
 #error "Can only be included in optimized version at the moment"
#endif
#endif


#include <vector>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class VertexPair;
// ----------------------------------------------------------------------------
class VertexPairHeap
{
public:
	VertexPairHeap();
	~VertexPairHeap();

public:
	void Insert(VertexPair* parVertexPair);
	VertexPair* ExtractMin();
	void Delete(const std::vector<VertexPair*>& parDeletePairs);
	void Update(const std::vector<VertexPair*>& parUpdatePairs);

public:
	size_t Size() const { return tree_.size(); }
	bool Empty() const { return tree_.empty(); }

#ifdef _DEBUG
public:
	bool IsValid(size_t parInd = 0) const;
	size_t LinearFindInTree(const VertexPair* parPair) const;
#endif

private:
	void Swap_(size_t parIndA, size_t parIndB);
	size_t DownHeap_(size_t parInd);
	size_t UpHeap_(size_t parInd);

private:
	std::vector<VertexPair*> tree_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif