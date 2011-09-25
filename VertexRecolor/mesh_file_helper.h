#ifndef MESH_FILE_HELPER_
#define MESH_FILE_HELPER_


#include "geometry.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace MeshFileHelper
{
// ----------------------------------------------------------------------------
Mesh* LoadMeshFromAsciiPlyFileWithPosNormCol(const std::string& parFilename);
Mesh* LoadMeshFromBinaryPlyFileWithPosAndTri(const std::string& parFilename);
void SaveMeshToBinaryPlyFile(const Mesh& parMesh, const std::string& parFilename);
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif
