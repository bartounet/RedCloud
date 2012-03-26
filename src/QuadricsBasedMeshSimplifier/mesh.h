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

#ifndef MESH_H_
#define MESH_H_


#include "../common/geometry.h"
#include "vertex.h"
#include "face.h"
#include "vertex_pair_heap.h"
#include <vector>
#include <set>


namespace QBMS
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Mesh
{
public:
	typedef std::pair<Vertex*, Vertex*> PairType;

public:
	Mesh(const Com::Mesh& parVRMesh);
	~Mesh();

public:
	void ExportToVRMesh(Com::Mesh& parDstMesh) const;
	void ComputeInitialQuadrics();
	void SelectAndComputeVertexPairs();
	void Simplify(uint parMaxFaces);
	void Clean();
	void SetDeleteUnusedVerticesAndReassignVerticesId();
	void DeleteFacesIFN();
	void DeleteVerticesIFN();
	void ComputeNormals(std::vector<Com::Vec4>& parNormals);

public:
	bool HasZeroAreaSurfaceFaces() const; // can be slow
	uint NbValidFaces() const;
	std::vector<Face*>& Faces();

private:
	void GenerateAdjacency_();
	uint NbValidFaces_() const;
	void MergeCloseVertices_(	const std::vector<double>& parNNDists,
								const std::vector<Vertex*> parNNs,
								double parMergeDist);
	void MarkIsolatedVerticesAndFacesToDelete_(	const std::vector<double>& parNNDists,
												double parIsolateDist);
	void MarkDegeneratedFacesToDelete_();
	void GaussianClean(float coef);

private:
	std::vector<Vertex*> vertices_;
	std::vector<Face*> faces_;

	VertexPairHeap pairsHeap_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}


#endif 
