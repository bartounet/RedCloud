#ifndef VERTEX_H_
#define VERTEX_H_


#include "../VertexRecolor/types.h"
#include "../VertexRecolor/geometry.h"
#include "face.h"
#include "quadric.h"
#include <vector>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Vertex
{
public:
	Vertex(float parX, float parY, float parZ);
	Vertex(const VR::Vertex& parVertex);
	~Vertex();

public:
	void AddIncidentFace(const Face* parFace);

public:
	const VR::Vec4& Pos() const { return pos_; }
	const std::vector<const Face*>& IncidentFaces() const { return incidentFaces_; }
	const Quadric& AssociatedQuadric() const { assert(associatedQuadric_); return *associatedQuadric_; }
	void SetAssociatedQuadric(Quadric* parQuadric) { associatedQuadric_ = parQuadric; }

private:
	VR::Vec4 pos_;
	std::vector<const Face*> incidentFaces_;
	Quadric* associatedQuadric_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif