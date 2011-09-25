
#include "mesh_file_helper.h"
#include <fstream>
#include <stdio.h>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace MeshFileHelper
{
// ----------------------------------------------------------------------------
#define CHECK_STRING(parStr, parStream, parBuff, parMaxLen) \
	parStream.getline(parBuff, parMaxLen, '\n');\
	assert(strcmp(parBuff, parStr) == 0);
#define CHECK_N_STRING(parStr, parStream, parBuff, parMaxLen) \
	parStream.getline(parBuff, parMaxLen, '\n');\
	assert(strncmp(parBuff, parStr, strlen(parStr)) == 0);
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromAsciiPlyFileWithPosNormCol(const std::string& parFilename)
{
	std::ifstream fileStream(parFilename.c_str(), std::ifstream::in);
	if (!fileStream.is_open())
		return 0;

	Mesh* mesh = new Mesh;
	assert(mesh);

	char buff[512];
	CHECK_STRING("ply", fileStream, buff, 512);
	CHECK_STRING("format ascii 1.0", fileStream, buff, 512);
	CHECK_N_STRING("element vertex ", fileStream, buff, 512);
	// FIXME: Recup le nombre de vertex
	CHECK_STRING("property float x", fileStream, buff, 512);
	CHECK_STRING("property float y", fileStream, buff, 512);
	CHECK_STRING("property float z", fileStream, buff, 512);
	CHECK_STRING("property float nx", fileStream, buff, 512);
	CHECK_STRING("property float ny", fileStream, buff, 512);
	CHECK_STRING("property float nz", fileStream, buff, 512);
	CHECK_STRING("property uchar diffuse_red", fileStream, buff, 512);
	CHECK_STRING("property uchar diffuse_green", fileStream, buff, 512);
	CHECK_STRING("property uchar diffuse_blue", fileStream, buff, 512);
	CHECK_STRING("end_header", fileStream, buff, 512);

	// FIXME

	fileStream.close();

	return mesh;
}
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromBinaryPlyFileWithPosAndTri(const std::string& parFilename)
{
	// FIXME

	return 0;
}
// ----------------------------------------------------------------------------
void SaveMeshToBinaryPlyFile(const Mesh& parMesh, const std::string& parFilename)
{
	// FIXME
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
