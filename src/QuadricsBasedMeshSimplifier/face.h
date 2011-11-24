#ifndef FACE_H_
#define FACE_H_


#include "../common/geometry.h"


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
	bool IsDegenerated() const;
	bool HasZeroAreaSurface() const;

public:
	void RemoveOnRelatedVertex();

public:
	Vertex* V0() const { return v0_; }
	Vertex* V1() const { return v1_; }
	Vertex* V2() const { return v2_; }
	void SetV0(Vertex* parVertex) { assert(parVertex); v0_ = parVertex; }
	void SetV1(Vertex* parVertex) { assert(parVertex); v1_ = parVertex; }
	void SetV2(Vertex* parVertex) { assert(parVertex); v2_ = parVertex; }

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