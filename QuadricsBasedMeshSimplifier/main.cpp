// Quadrics based mesh simplifier

#include "../VertexRecolor/geometry.h"
#include "../VertexRecolor/mesh_file_helper.h"
#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>


void Usage(const char* parProgName)
{
	printf("Usage: ./%s <MESH_IN.ply> <MESH_OUT.ply>\n", parProgName);
	exit(1);
}

int main(int argc, char **argv)
{
#ifdef _DEBUG
	printf("=== Quadrics based mesh simplifier (DEBUG MODE)===\n");
#else
	printf("=== Quadrics based mesh simplifier ===\n");
#endif

	if (argc != 3)
		Usage(argv[0]);

	printf("[ ] Loading '%s' mesh file\n", argv[1]);
	VR::Mesh srcMesh;
	VR::MeshFileHelper::LoadMeshFromPlyFile(srcMesh, argv[1]);
	printf("[+] Mesh loaded\n");

	QBMS::Mesh mesh(srcMesh);

	mesh.ComputeInitialQuadrics();
	mesh.SelectAndComputeVertexPairs();
	mesh.Simplify(20000);

	VR::Mesh dstMesh;
	mesh.ExportToVRMesh(dstMesh);

	printf("\t[.] Vertices reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh.vertices.size() / (float)srcMesh.vertices.size()));
	printf("\t[.] Faces reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh.faces.size() / (float)srcMesh.faces.size()));

	printf("[ ] Saving '%s' mesh file (vertices: %d, faces: %d)\n", argv[2], dstMesh.vertices.size(), dstMesh.faces.size());
	VR::MeshFileHelper::SaveMeshToPlyFile(dstMesh, argv[2], true);
	printf("[+] Mesh saved\n");

	// FIXME: As long as mesh destructor (and its attribute) is empty,
	// we avoid a massive call to empty destructor. But we leak...
	exit(0);

	return 0;
}