//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#include "../common/mesh_file_helper.h"
#include "vertex_recolor.h"
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
	Com::Mesh coloredMesh;
	result = Com::MeshFileHelper::LoadMeshFromPlyFile(coloredMesh, argv[1 + argOffset]);
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
	Com::Mesh triMesh;
	result = Com::MeshFileHelper::LoadMeshFromPlyFile(triMesh, argv[2 + argOffset]);
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
	Com::Mesh finalMesh;
	srand(0);
	VR::VertexRecolor(finalMesh, coloredMesh, triMesh);
	if (verbose)
		printf("[+] Color recovered\n");

	result = Com::MeshFileHelper::SaveMeshToPlyFile(finalMesh, argv[3 + argOffset], false);
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
	Com::MeshFileHelper::SaveMeshToPlyFile(finalMesh, "easy_cleaned.ply", false);
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
