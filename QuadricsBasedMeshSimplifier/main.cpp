// Quadrics based mesh simplifier

#include "../VertexRecolor/geometry.h"
#include "../VertexRecolor/mesh_file_helper.h"
#include "mesh.h"
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

	printf("[ ] Loading '%s' mesh file\n", argv[1]);
	VR::Mesh srcMesh;
	VR::MeshFileHelper::LoadMeshFromPlyFile(srcMesh, argv[1]);
	printf("[+] Mesh loaded\n");

	QBMS::Mesh mesh(srcMesh);
	mesh.ComputeInitialQuadrics();
	mesh.SelectAndComputeVertexPairs();
	mesh.Simplify();

	VR::Mesh* dstMesh = mesh.ExportToVRMesh();

	printf("[ ] Saving '%s' mesh file\n", argv[2]);
	VR::MeshFileHelper::SaveMeshToPlyFile(*dstMesh, argv[2], true);
	printf("[+] Mesh saved\n");

	delete dstMesh;

	return 0;
}