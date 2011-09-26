
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

	ply_close(plyFile);

	return mesh;
}
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromAsciiPlyFileWithPosAndFace(char* parFilename)
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
	assert(plyFile->nelems == 2);
	assert(equal_strings(plyFile->elems[0]->name, "vertex"));
	assert(equal_strings(plyFile->elems[1]->name, "face"));

	PlyProperty vertProps[] =
	{
		{"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
		{"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
		{"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
	};
	int nbVertices;
	int nbVertexProps;
	ply_get_element_description(plyFile, "vertex", &nbVertices, &nbVertexProps);
	assert(nbVertices > 0);
	assert(nbVertexProps == 3);
	for (int prop = 0; prop < nbVertexProps; ++prop)
		ply_get_property(plyFile, "vertex", &vertProps[prop]);

	Mesh* mesh = new Mesh;
	for (int vertex = 0; vertex < nbVertices; ++vertex)
	{
		Vertex curVertex;
		ply_get_element(plyFile, (void*) &curVertex);
		curVertex.nx = curVertex.ny = curVertex.nz = 0.f;
		curVertex.r = 255;
		curVertex.g = 0;
		curVertex.b = 0;
		mesh->vertices.push_back(curVertex);
	}

	PlyProperty face_props[] =
	{
		{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,vertices),
			1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nbVertices)},
	};
	int nbFaces;
	int nbFaceProps;
	PlyProperty** facesProps = ply_get_element_description(plyFile, "face", &nbFaces, &nbFaceProps);
	assert(nbFaces > 0);
	assert(nbFaceProps == 1);

	ply_get_property(plyFile, "face", &face_props[0]);

	for (int face = 0; face < nbFaces; ++face)
	{
		Face curFace;
		ply_get_element(plyFile, (void*) &curFace);
		assert(curFace.nbVertices == 3);
		mesh->faces.push_back(curFace);
	}

	ply_close(plyFile);
	
	return mesh;
}
// ----------------------------------------------------------------------------
void SaveMeshToAsciiPlyFile(const Mesh& parMesh, char* parFilename)
{
	char* elementNames[] = {"vertex"};
	float version;
	PlyFile* plyFile = ply_open_for_writing(parFilename, 1, elementNames, PLY_ASCII, &version);
	assert(plyFile);
	int nbVertices = parMesh.vertices.size();
	assert(nbVertices > 0);

	// header
	ply_element_count(plyFile, "vertex", nbVertices);
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
	for (int prop = 0; prop < 9; ++prop)
		ply_describe_property(plyFile, "vertex", &vertProps[prop]);
	ply_put_comment(plyFile, "Generated by VertexRecolor from the RedCloud team");
	ply_header_complete(plyFile);

	// data
	ply_put_element_setup(plyFile, "vertex");
	for (int vertex = 0; vertex < nbVertices; ++vertex)
		ply_put_element(plyFile, (void*) &parMesh.vertices[vertex]);

	ply_close(plyFile);
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
