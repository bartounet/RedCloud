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

#ifndef VERTEX_H_
#define VERTEX_H_


#include "../common/types.h"
#include "../common/geometry.h"
#include "quadric.h"
#include <vector>
#include <assert.h>


#define BAD_VERTEX_ID 999999999


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class VertexPair;
class Face;
class Vertex
{
public:
	typedef std::vector<VertexPair*> PairListType;
	typedef std::vector<Face*> FaceListType;

public:
	Vertex(const Com::Vertex& parVertex, uint parId);
	~Vertex();

public:
	void AddIncidentFace(Face* parFace);
	void AddIncidentFaces(const FaceListType& parFaceList);
	void AddPair(VertexPair* parPair);
	void AddPairs(const PairListType& parPairList);

	void ReplaceThisInIncidentFacesWith(Vertex* parNewVertex);
	void ReplaceThisInPairsWith(Vertex* parNewVertex);

	void UpdatePairPosAndQuadric(std::vector<VertexPair*>& parUpdatePairs);

	void RemoveIncidentFace(Face* parFace);
	void RemoveIncidentFaceIFN(Face* parFace);
	uint RemoveDegeneratedFaces();
	void RemovePair(VertexPair* parPair);
	void RemoveInvalidPair(std::vector<VertexPair*>& parDeletePairs);
	void RemoveDuplicatedPair(std::vector<VertexPair*>& parDeletePairs);
	void RemoveDegeneratedPairs(std::vector<VertexPair*>& parDeletePairs);
	void SetDeleteMeOnRelatedFaces();

public:
	inline const Com::Vec4& Pos() const { return pos_; }
	inline const FaceListType& IncidentFaces() const { return incidentFaces_; }
	inline const Quadric& GetQuadric() const { return quadric_; }
	inline uint Id() const { return id_; }
	const PairListType& Pairs() const { return pairs_; }
	inline bool DeleteMe() const { return deleteMe_; }

public:
	inline void SetId(uint parId) { id_ = parId; }
	inline void SetDeleteMe() { deleteMe_ = true; }
	inline void SetPos(const Com::Vec4& parPos) { pos_ = parPos; }
#ifdef _DEBUG
	void SetQuadric(const Quadric& parQuadric);
#else
	inline void SetQuadric(const Quadric& parQuadric) { quadric_ = parQuadric; }
#endif

private:
	FaceListType incidentFaces_;
	PairListType pairs_;

	Com::Vec4 pos_;
	Quadric quadric_;
	
	uint id_;
	bool deleteMe_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif