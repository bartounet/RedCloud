
#include "mesh_file_helper.h"
#include "ply.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace MeshFileHelper
{
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromAsciiPlyFileWithPosNormCol(char* parFilename)
{
	assert(parFilename);

	int nelems;
	char** elem_names;
	int file_type;
	float version;
	PlyFile* plyFile = ply_open_for_reading(parFilename, &nelems, &elem_names, &file_type, &version);
	assert(plyFile);
	assert(plyFile->file_type == PLY_ASCII);
	assert(plyFile->version == 1.0f);
	assert(plyFile->nelems == 1);
	assert(equal_strings(plyFile->elems[0]->name, "vertex"));

	PlyProperty vertProps[] =
	{
		{"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
		{"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
		{"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
		{"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
		{"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
		{"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
		{"diffuse_red", PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,r), 0, 0, 0, 0},
		{"diffuse_green", PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,g), 0, 0, 0, 0},
		{"diffuse_blue", PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,b), 0, 0, 0, 0},
	};
	
	int nbVertices;
	int nbProps;
	ply_get_element_description(plyFile, "vertex", &nbVertices, &nbProps);
	assert(nbVertices > 0);
	assert(nbProps == 9);
	for (int prop = 0; prop < nbProps; ++prop)
		ply_get_property(plyFile, "vertex", &vertProps[prop]);

	Mesh* mesh = new Mesh;
	for (int vertex = 0; vertex < nbVertices; ++vertex)
	{
		Vertex curVertex;
		ply_get_element(plyFile, (void*) &curVertex);
		mesh->vertices.push_back(curVertex);
	}

	return mesh;
}
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromBinaryPlyFileWithPosAndTri(char* parFilename)
{
	// FIXME

	Mesh* mesh = new Mesh;
	return mesh;
}
// ----------------------------------------------------------------------------
void SaveMeshToBinaryPlyFile(const Mesh& parMesh, char* parFilename)
{
	// FIXME
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
