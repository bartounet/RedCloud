// Quadrics based mesh simplifier

#include "../VertexRecolor/geometry.h"
#include "../VertexRecolor/mesh_file_helper.h"
#include "simplifier.h"
#include <stdio.h>
#include <stdlib.h>


void Usage(const char* parProgName)
{
	printf("Usage: ./%s <MESH_IN.ply> <MESH_OUT.ply>\n");
	exit(1);
}

int main(int argc, char **argv)
{
	printf("=== Quadrics based mesh simplifier ===\n");

	if (argc != 3)
		Usage(argv[0]);

	printf("[ ] Loading mesh\n");
	VR::Mesh srcMesh;
	VR::MeshFileHelper::LoadMeshFromPlyFile(srcMesh, argv[1]);
	printf("[+] Mesh loaded\n");

	QBMS::Mesh detailedMesh(srcMesh);
	detailedMesh.ComputeInitialQuadrics();

	VR::Mesh* dstMesh = detailedMesh.ExportToVRMesh();

	printf("[ ] Saving mesh\n");
	VR::MeshFileHelper::SaveMeshToPlyFile(*dstMesh, argv[2], true);
	printf("[+] Mesh saved\n");

	delete dstMesh;

	return 0;
}