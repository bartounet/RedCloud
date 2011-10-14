#ifndef FACE_H_
#define FACE_H_


#include "../VertexRecolor/geometry.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Vertex;
class Face
{
public:
	Face();
	Face(Vertex* parV0, Vertex* parV1, Vertex* parV2);
	~Face();

public:
	Vertex* V0() const { return v0_; }
	Vertex* V1() const { return v1_; }
	Vertex* V2() const { return v2_; }

private:
	Vertex* v0_;
	Vertex* v1_;
	Vertex* v2_;
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 