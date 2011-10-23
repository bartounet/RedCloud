// Quadrics based mesh simplifier

#include "../VertexRecolor/geometry.h"
#include "../VertexRecolor/mesh_file_helper.h"
#include "mesh.h"
#include "updatable_heap.h"
#include <stdio.h>
#include <stdlib.h>


void Usage(const char* parProgName)
{
	printf("Usage: ./%s <MESH_IN.ply> <MESH_OUT.ply>\n");
	exit(1);
}

int main(int argc, char **argv)
{
#if 1
	QBMS::UpdatableHeap<int> heap;
	heap.Insert(2);
	heap.Insert(1);
	heap.Insert(5);
	heap.Insert(4);
	heap.Insert(3);
	heap.Insert(3);
	assert(heap.IsValid());

	printf("extract: %d\n", heap.ExtractMin());
	assert(heap.IsValid());

	printf("extract: %d\n", heap.ExtractMin());
	assert(heap.IsValid());

	heap.Insert(10);
	heap.Insert(1);
	heap.Insert(0);
	assert(heap.IsValid());

	// FIXME: Tester la mise a jour

#else
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

	printf("\t[.] Vertices reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh->vertices.size() / (float)srcMesh.vertices.size()));
	printf("\t[.] Faces reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh->faces.size() / (float)srcMesh.faces.size()));

	printf("[ ] Saving '%s' mesh file (vertices: %d, faces: %d)\n", argv[2], dstMesh->vertices.size(), dstMesh->faces.size());
	VR::MeshFileHelper::SaveMeshToPlyFile(*dstMesh, argv[2], true);
	printf("[+] Mesh saved\n");

	delete dstMesh;

	system("pause");
#endif
	return 0;
}