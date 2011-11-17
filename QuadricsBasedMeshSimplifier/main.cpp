// Quadrics based mesh simplifier

#include "../VertexRecolor/geometry.h"
#include "../VertexRecolor/mesh_file_helper.h"
#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // GetTickCount()


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

	size_t beforeSimplyTime = GetTickCount();

	mesh.ComputeInitialQuadrics();
	mesh.SelectAndComputeVertexPairs();
	mesh.Simplify(20000);

	size_t afterSimplyTime = GetTickCount();
	size_t elapsedSimplyTime = (afterSimplyTime - beforeSimplyTime) / 1000; // in seconds

	size_t hours = elapsedSimplyTime / 3600;
	elapsedSimplyTime -= hours * 3600;
	size_t minutes = elapsedSimplyTime / 60;
	elapsedSimplyTime -= minutes * 60;
	size_t seconds = elapsedSimplyTime;
	
	printf("[+] Simplification done in %dh %dm %ds\n", hours, minutes, seconds);

	VR::Mesh* dstMesh = mesh.ExportToVRMesh();

	printf("\t[.] Vertices reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh->vertices.size() / (float)srcMesh.vertices.size()));
	printf("\t[.] Faces reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh->faces.size() / (float)srcMesh.faces.size()));

	printf("[ ] Saving '%s' mesh file (vertices: %d, faces: %d)\n", argv[2], dstMesh->vertices.size(), dstMesh->faces.size());
	VR::MeshFileHelper::SaveMeshToPlyFile(*dstMesh, argv[2], true);
	printf("[+] Mesh saved\n");

	delete dstMesh;

	system("pause");
	return 0;
}