#ifndef VERTEX_PAIR_HEAP_H_


#include "../VertexRecolor/types.h"
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
	uint Size() const { return tree_.size(); }
	bool Empty() const { return tree_.empty(); }

#ifdef _DEBUG
public:
	bool IsValid(uint parInd = 0) const;
	uint LinearFindInTree(const VertexPair* parPair) const;
#endif

private:
	void Swap_(uint parIndA, uint parIndB);
	uint DownHeap_(uint parInd);
	uint UpHeap_(uint parInd);

private:
	std::vector<VertexPair*> tree_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif