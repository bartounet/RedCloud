//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#ifndef VERTEX_PAIR_HEAP_H_


#include "../common/types.h"
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