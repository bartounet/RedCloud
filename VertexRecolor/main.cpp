
#include "mesh_file_helper.h"
#include "vertex_recolor.h"
#include <stdio.h>
#include <stdlib.h>
	

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void Usage(const std::string& parExecFilename)
{
	printf("Usage: %s [-v] <COLORED_FILE> <TRIANGLE_FILE> <OUTPUT_FILE>\n", parExecFilename.c_str());
	exit(1);
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	printf("=== VERTEX RECOLOR ===\n");

	if (argc < 4 || argc > 5)
		Usage(argv[0]);

	bool verbose = false;
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
	
	if (verbose)
		printf("[ ] Loading colored mesh...\n");
	Mesh* coloredMesh = MeshFileHelper::LoadMeshFromAsciiPlyFileWithPosNormCol(argv[1 + argOffset]);
	if (!coloredMesh)
	{
		if (verbose)
			printf("[-] Failed to load colored mesh '%s'\n", argv[1 + argOffset]);
		exit(2);
	}
	else if (verbose)
		printf("[+] Colored Mesh loaded\n");

	if (verbose)
		printf("[ ] Loading triangularized mesh...\n");
	Mesh* triMesh = MeshFileHelper::LoadMeshFromAsciiPlyFileWithPosAndFace(argv[2 + argOffset]);
	if (!triMesh)
	{
		if (verbose)
			printf("[-] Failed to load triangularized mesh '%s'\n", argv[2 + argOffset]);
		delete coloredMesh;
		exit(3);
	}
	else if (verbose)
		printf("[+] Triangularized Mesh loaded\n");

	if (verbose)
		printf("[ ] Recovering color...\n");
	Mesh* finalMesh = VertexRecolor(*coloredMesh, *triMesh, verbose);
	delete coloredMesh;
	delete triMesh;
	if (!finalMesh)
	{
		if (verbose)
			printf("[-] Failed to recover color\n");
		exit(4);
	}
	else if (verbose)
		printf("[+] Color recovered\n");

	//FIXME//MeshFileHelper::SaveMeshToBinaryPlyFile(*finalMesh, argv[3 + argOffset]);
	if (verbose)
		printf("[+] Colored & Triangularized mesh is saved\n");
	delete finalMesh;

	if (verbose)
		printf("[+] Quitting!\n");
	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
