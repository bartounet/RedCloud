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

#ifndef THREE_D_TREE_H_
#define THREE_D_TREE_H_


#include "geometry.h"
#include <vector>


namespace Com
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
// ----------------------------------------------------------------------------
enum AlignedAxisDir
{
	AADIR_X,
	AADIR_Y,
	AADIR_Z
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Node
{
public:
	Node() {}
	virtual ~Node() {}
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
template <typename VertexType>
class ThreeDNode : public Node
{
public:
	ThreeDNode(const VertexType* parVertex, AlignedAxisDir parDir);
	virtual ~ThreeDNode();

public:
	static Node* BuildTree(const std::vector<const VertexType*>& parVertices, int parDepth, const uint parMaxVertexPerLeaf);
	static void NearestPoint(const Node* parNode,
							 const Vec4& parTargetPos,
							 int parDepth,
							 double& parMinDist,
							 const VertexType** parMinVertex);
	static void NearestNeighbour(	const Node* parTree,
									const VertexType& parTarget,
									int parDepth,
									double& parMinDist,
									const VertexType** parMinVertex);

private:
	AlignedAxisDir dir_;
	const VertexType* vertex_;
	Node* left_; // lesser and equal
	Node* right_; // greater
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
template <typename VertexType>
class LeafNode : public Node
{
public:
	LeafNode(const std::vector<const VertexType*> parVertices);
	virtual ~LeafNode();

	const std::vector<const VertexType*>& Vertices() const { return vertices_; }

private:
	std::vector<const VertexType*> vertices_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}

#include "threedtree.impl.hpp"


#endif