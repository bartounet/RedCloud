
#include "../common/mesh_file_helper.h"
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
	printf("Usage: %s [-v] <INPUT_PLYFILE> <OUTPUT_PLYFILE>\n", parExecFilename.c_str());
	exit(1);
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	printf("=== CLEANER ===\n");

	if (argc < 3 || argc > 4)
		Usage(argv[0]);

	bool verbose = false;
	bool result = false;
	int argOffset = 0;
	if (argc == 4)
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
		printf("[ ] Loading mesh to clean...\n");
	Com::Mesh mesh;
	result = Com::MeshFileHelper::LoadMeshFromPlyFile(mesh, argv[1 + argOffset]);
	if (!result)
	{
		if (verbose)
			printf("[-] Failed to load mesh to clean '%s'\n", argv[1 + argOffset]);
		exit(2);
	}
	else if (verbose)
		printf("[+] Mesh loaded\n");

	if (verbose)
		printf("[ ] Cleaning Mesh\n");
	EC::EasyClean(mesh);
	if (verbose)
	{
		printf("[+] Mesh cleaned\n");
		printf("[ ] Save cleaned mesh...\n");
	}
	Com::MeshFileHelper::SaveMeshToPlyFile(mesh, argv[2 + argOffset], false);
	if (verbose)
		printf("[+] Cleaned mesh saved\n");

	if (verbose)
		printf("[+] Quitting!\n");
	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
