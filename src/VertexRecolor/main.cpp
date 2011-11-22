
#include "mesh_file_helper.h"
#include "vertex_recolor.h"
#include "easy_cleaner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
	

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void Usage(const std::string& parExecFilename)
{
	printf("Usage: %s [-v] <COLORED_PLYFILE> <TRIANGLE_PLYFILE> <OUTPUT_PLYFILE>\n", parExecFilename.c_str());
	exit(1);
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	printf("=== VERTEX RECOLOR ===\n");

	if (argc < 4 || argc > 5)
		Usage(argv[0]);

	bool verbose = false;
	bool result = false;
	int argOffset = 0;
	if (argc == 5)
	{
		if (strncmp("-v", argv[1], 2) != 0)
			Usage(argv[0]);
		verbose = true;
		argOffset = 1;
	}
	else if ((strncmp("-v", argv[1], 2) == 0) ||
			(strncmp("-v", argv[2], 2) == 0) ||
			(strncmp("-v", argv[3], 2) == 0))
	{
		Usage(argv[0]);
	}
	
	// colored mesh
	if (verbose)
		printf("[ ] Loading colored mesh...\n");
	VR::Mesh coloredMesh;
	result = VR::MeshFileHelper::LoadMeshFromPlyFile(coloredMesh, argv[1 + argOffset]);
	if (!result)
	{
		if (verbose)
			printf("[-] Failed to load colored mesh '%s'\n", argv[1 + argOffset]);
		exit(2);
	}
	else if (verbose)
		printf("[+] Colored Mesh loaded\n");

	// triangularized mesh
	if (verbose)
		printf("[ ] Loading triangularized mesh...\n");
	VR::Mesh triMesh;
	result = VR::MeshFileHelper::LoadMeshFromPlyFile(triMesh, argv[2 + argOffset]);
	if (!result)
	{
		if (verbose)
			printf("[-] Failed to load triangularized mesh '%s'\n", argv[2 + argOffset]);
		exit(3);
	}
	else if (verbose)
		printf("[+] Triangularized Mesh loaded\n");

	// compute the final mesh
	if (verbose)
		printf("[ ] Recovering color...\n");
	VR::Mesh finalMesh;
	srand(0);
	VR::VertexRecolor(finalMesh, coloredMesh, triMesh);
	if (verbose)
		printf("[+] Color recovered\n");

	result = VR::MeshFileHelper::SaveMeshToPlyFile(finalMesh, argv[3 + argOffset], false);
	if (!result)
	{
		if (verbose)
			printf("[-] Failed to save Colored & Triangularized mesh\n");
		exit(5);
	}
	else if (verbose)
		printf("[+] Colored & Triangularized mesh is saved\n");

#if 0
	if (verbose)
		printf("[ ] Cleaning Mesh\n");
	VR::EasyClean(finalMesh);
	if (verbose)
	{
		printf("[+] Mesh cleaned\n");
		printf("[ ] Save cleaned mesh...\n");
	}
	VR::MeshFileHelper::SaveMeshToPlyFile(finalMesh, "easy_cleaned.ply", false);
	if (verbose)
		printf("[+] Cleaned mesh saved\n");
#endif

	if (verbose)
		printf("[+] Quitting!\n");
	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================