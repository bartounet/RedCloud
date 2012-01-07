#ifndef MESH_FILE_HELPER_
#define MESH_FILE_HELPER_

#include "geometry.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace Com
{
// ----------------------------------------------------------------------------
namespace MeshFileHelper
{
// ----------------------------------------------------------------------------
bool LoadMeshFromPlyFile(Mesh& parMesh, char* parFilename);
bool SaveMeshToPlyFile(const Mesh& parMesh, char* parFilename, bool parOnlyPos);
bool SaveMeshToDaeFile(const Mesh& parMesh, std::vector<std::vector<Vec2> > texCoords, uint textureSize, const char* parFilename, std::vector<const Com::Vec4>& vertexNormals);
void DeepCopyMesh(Mesh& parMeshDst, const Mesh& parMeshSrc);
// ----------------------------------------------------------------------------
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif
