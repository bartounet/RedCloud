#include <stdio.h>
#include <stdlib.h>
#include "mesh_file_helper.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Usage: %s <COLORED_FILE> <TRIANGLE_FILE> <OUTPUT_FILE>\n", argv[0]);
		exit(1);
	}

	const Mesh* coloredMesh = MeshFileHelper::LoadMeshFromAsciiPlyFileWithPosNormCol(argv[1]);
	const Mesh* triMesh = MeshFileHelper::LoadMeshFromBinaryPlyFileWithPosAndTri(argv[2]);

	const Mesh* finalMesh = 0; // FIXME

	MeshFileHelper::SaveMeshToBinaryPlyFile(*finalMesh, argv[3]);

	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
