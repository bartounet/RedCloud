// Quadrics based mesh simplifier

#include "../common/geometry.h"
#include "../common/mesh_file_helper.h"
#include "mesh.h"
#include "texturer.h"
#include <stdio.h>
#include <stdlib.h>


void Usage(const char* parProgName)
{
	printf("Usage: ./%s <MESH_IN.ply> <POINTS_CLOUD_IN.ply> <OUT_FOLDER> numberOfFaces textureSize\n", parProgName);
	printf("Usage: ./%s <MESH_IN.ply> <MESH_OUT.ply> --clean\n", parProgName);
	exit(1);
}

int main(int argc, char **argv)
{
#ifdef _DEBUG
	printf("=== Quadrics based mesh simplifier (DEBUG MODE) ===\n");
#else
 #ifndef NDEBUG
	printf("=== Quadrics based mesh simplifier (RELEASE MODE) ===\n");
 #else
	printf("=== Quadrics based mesh simplifier (FINAL MODE) ===\n");
 #endif
#endif

	if (((argc != 6) && (argc != 4)) || ((argc == 4) && (strcmp(argv[3], "--clean"))))
		Usage(argv[0]);

	printf("- Loading '%s' mesh file\n", argv[1]);
	Com::Mesh srcMesh;
	bool result = Com::MeshFileHelper::LoadMeshFromPlyFile(srcMesh, argv[1]);
	assert(result);

	QBMS::Mesh mesh(srcMesh);

	printf("- Cleaning mesh\n");
	mesh.Clean();

	if (argc == 4)
	{
		Com::Mesh dstMesh;
		mesh.ExportToVRMesh(dstMesh);
		Com::MeshFileHelper::SaveMeshToPlyFile(dstMesh, argv[2], true);
		exit(0);
	}

	printf("- Checking zero area faces\n");
	if (mesh.HasZeroAreaSurfaceFaces())
	{
		printf("- Zero area faces found! Abort...\n");
		exit(2);
	}

	mesh.ComputeInitialQuadrics();
	mesh.SelectAndComputeVertexPairs();
	int numberOfFaces = atoi (argv[4]);
	mesh.Simplify(numberOfFaces);

	//assert(!mesh.HasZeroAreaSurfaceFaces()); // FIXME: Post CLEAN !
#if 0
	printf("[ ] Mesh post-clean\n");
	mesh.Clean(false, false, true, true);
	printf("[+] Mesh post-cleaned\n");
	assert(!mesh.HasZeroAreaSurfaceFaces()); // FIXME: Post CLEAN !
#endif

	Com::Mesh dstMesh;
	mesh.ExportToVRMesh(dstMesh);

	printf("\t- Vertices reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh.vertices.size() / (float)srcMesh.vertices.size()));
	printf("\t- Faces reduction: %.2f percent\n",
		100.f * (1.f - (float)dstMesh.faces.size() / (float)srcMesh.faces.size()));



	printf("- Generate texture\n");
	Texturer texturer(atoi(argv[5]));

	printf("\t- Create mapping\n");
	texturer.CreateMapping(mesh.NbValidFaces());

	printf("\t- Loading '%s' points cloud file\n", argv[2]);
	Com::Mesh pointsCloud;
	result = Com::MeshFileHelper::LoadMeshFromPlyFile(pointsCloud, argv[2]);
	assert(result);
	printf("\t- Filling texture\n");
	texturer.Fill(mesh.Faces (), pointsCloud);

	printf("\t- Dumping texture\n");
	std::string textureName = std::string(argv[3]) + "texture.ppm";
	result = texturer.DumpTexture(textureName);
	assert(result);

	printf("\t- Generate vertex normals\n");
	std::vector<Com::Vec4> vertexNormals = std::vector<Com::Vec4>();
	mesh.ComputeNormals(vertexNormals);

	printf("- Saving mesh file (vertices: %d, faces: %d)\n", dstMesh.vertices.size(), dstMesh.faces.size());
	std::string outDae = std::string(argv[3]) + "model.dae";
	Com::MeshFileHelper::SaveMeshToDaeFile(dstMesh, texturer.GetTexCoords(), atoi(argv[5]), outDae.c_str(), vertexNormals);


	exit(0);	// FIXME: As long as mesh destructor (and its attribute) is empty,
				// we avoid a massive call to empty destructor. But it leak...

	return 0;
}
