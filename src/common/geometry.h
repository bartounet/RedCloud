#ifndef GEOMETRY_H_
#define GEOMETRY_H_


#include "types.h"
#include <vector>
#include <stdio.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace Com
{
// ----------------------------------------------------------------------------
struct Vertex
{
	float x, y, z; // position
	float nx, ny, nz; // normal
	uchar r, g, b; // color

	Vertex(const Vec4& parPos) :
		x((float)parPos.x), y((float)parPos.y), z((float)parPos.z),
		nx(0.0f), ny(0.0f), nz(0.0f),
		r(255), g(0), b(0)
	{
	}

	Vertex() :
		x(0.0f), y(0.0f), z(0.0f),
		nx(0.0f), ny(0.0f), nz(0.0f),
		r(255), g(0), b(0)
	{
	}
};
// ----------------------------------------------------------------------------
// Bart: As we only work with triangles I would prefer another form, but we have
// to follow the ply file format style (... then the parsing is easier ...)
struct Face 
{
	int *vertices;
	uchar nbVertices;

	Face(uint parV0, uint parV1, uint parV2)
	{
		nbVertices = (uchar) 3;

		vertices = new int[3];
		vertices[0] = (int) parV0;
		vertices[1] = (int) parV1;
		vertices[2] = (int) parV2;
	}

	Face() :
		vertices(0),
		nbVertices(0)
	{
	}
};
// ----------------------------------------------------------------------------
struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	~Mesh() // because the ply loader allocate
	{
		printf("[ ] Freeing mesh memory\n");
		for (uint face = 0; face < faces.size(); ++face)
			delete faces[face].vertices;
		printf("[+] Mesh freed\n");
	}
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif