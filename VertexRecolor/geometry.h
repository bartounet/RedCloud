#ifndef GEOMETRY_H_
#define GEOMETRY_H_


#include "types.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
struct Vertex
{
	float x, y, z; // position
	float nx, ny, nz; // normal
	uchar r, g, b; // color
};
// ----------------------------------------------------------------------------
// Bart: As we only work with triangles I would prefer another form, but we have
// to follow the ply file format style (... then the parsing is easier ...)
struct Face 
{
	int *vertices;
	uchar nbVertices;
};
// ----------------------------------------------------------------------------
struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	~Mesh() // because the ply loader allocate
	{
		for (size_t face = 0; face < faces.size(); ++face)
			delete faces[face].vertices;
	}
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif