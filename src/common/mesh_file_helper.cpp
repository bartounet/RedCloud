
#include "mesh_file_helper.h"
#include "ply.h"
#include <assert.h>
#include <stdlib.h>
#include <fstream>


// ----------------------------------------------------------------------------
namespace Com
{
// ----------------------------------------------------------------------------
namespace MeshFileHelper
{
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
static PlyProperty vertProps[] =
{
	{(char*)"x", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,x), 0, 0, 0, 0},
	{(char*)"y", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,y), 0, 0, 0, 0},
	{(char*)"z", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,z), 0, 0, 0, 0},
	{(char*)"nx", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,nx), 0, 0, 0, 0},
	{(char*)"ny", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,ny), 0, 0, 0, 0},
	{(char*)"nz", PLY_FLOAT, PLY_FLOAT, offsetof(VertexPOD,nz), 0, 0, 0, 0},
	{(char*)"diffuse_red", PLY_UCHAR, PLY_UCHAR, offsetof(VertexPOD,r), 0, 0, 0, 0},
	{(char*)"diffuse_green", PLY_UCHAR, PLY_UCHAR, offsetof(VertexPOD,g), 0, 0, 0, 0},
	{(char*)"diffuse_blue", PLY_UCHAR, PLY_UCHAR, offsetof(VertexPOD,b), 0, 0, 0, 0},
};
// ----------------------------------------------------------------------------
static PlyProperty faceProps[] =
{
	{(char*)"vertex_indices", PLY_INT, PLY_INT, offsetof(FacePOD,vertices),
		1, PLY_UCHAR, PLY_UCHAR, offsetof(FacePOD,nbVertices)},
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
bool LoadMeshFromPlyFile(Mesh& parMesh, char* parFilename)
{
	assert(parFilename);
	assert(parMesh.vertices.size() == 0);
	assert(parMesh.faces.size() == 0);

	// prepare file
	int nelems;
	char** elem_names;
	int file_type;
	float version;
	PlyFile* plyFile = ply_open_for_reading(parFilename, &nelems, &elem_names, &file_type, &version);
	if (!plyFile)
		return false;
	assert(plyFile->file_type == PLY_ASCII);
	assert(plyFile->version == 1.0f);
	assert((plyFile->nelems == 1) || (plyFile->nelems == 2));
	assert(equal_strings(plyFile->elems[0]->name, "vertex"));
	if (plyFile->nelems == 2)
		assert(equal_strings(plyFile->elems[1]->name, "face"));

	// get vertices
	int nbVertices;
	int nbVertexProps;
	ply_get_element_description(plyFile, "vertex", &nbVertices, &nbVertexProps);
	assert(nbVertices > 0);
	assert(nbVertexProps == 3 || nbVertexProps == 9); // pos only or pos/normal/color
	for (int prop = 0; prop < nbVertexProps; ++prop)
		ply_get_property(plyFile, "vertex", &vertProps[prop]);
	for (int vertex = 0; vertex < nbVertices; ++vertex)
	{
		Vertex curVertex;
		ply_get_element(plyFile, (void*) &curVertex);
		if (nbVertexProps == 3)
		{
			curVertex.nx = curVertex.ny = curVertex.nz = 0.f;
			curVertex.r = 255;
			curVertex.g = 0;
			curVertex.b = 0;
		}
		parMesh.vertices.push_back(curVertex);
	}

	// get faces
	if (plyFile->nelems == 2)
	{
		int nbFaces = 0;
		int nbFaceProps = 0;
		ply_get_element_description(plyFile, "face", &nbFaces, &nbFaceProps);
		assert(nbFaces > 0);
		assert(nbFaceProps == 1);

		ply_get_property(plyFile, "face", &faceProps[0]);
		for (int face = 0; face < nbFaces; ++face)
		{
			Face curFace;
			ply_get_element(plyFile, (void*) &curFace);
			assert(curFace.nbVertices == 3); // mesh should contain only triangles
			parMesh.faces.push_back(curFace);
		}
	}

	ply_close(plyFile);

	return true;
}
// ----------------------------------------------------------------------------
bool SaveMeshToDaeFile(const Mesh& parMesh, std::vector<std::vector<Vec2> > texCoords, uint textureSize, const char* parFilename, std::vector<const Com::Vec4>& vertexNormals)
{
	std::ofstream fichier(parFilename, std::ios::out | std::ios::trunc);

    if (fichier)
    {
        fichier << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        fichier << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">" << std::endl;
        fichier << "    <asset>" << std::endl;
        fichier << "        <contributor>" << std::endl;
        fichier << "            <author>VCGLab</author>" << std::endl;
        fichier << "            <authoring_tool>VCGLib | MeshLab</authoring_tool>" << std::endl;
        fichier << "        </contributor>" << std::endl;
        fichier << "        <up_axis>Z_UP</up_axis>" << std::endl;
        fichier << "        <created>jeu. 10. nov. 12:48:09 2011</created>" << std::endl;
        fichier << "        <modified>jeu. 10. nov. 12:48:09 2011</modified>" << std::endl;
        fichier << "    </asset>" << std::endl;
        fichier << "    <library_images>" << std::endl;
        fichier << "        <image id=\"texture0\" name=\"texture0\">" << std::endl;
        fichier << "            <init_from>texture.png</init_from>" << std::endl;
        fichier << "        </image>" << std::endl;
        fichier << "    </library_images>" << std::endl;
        fichier << "    <library_materials>" << std::endl;
        fichier << "        <material id=\"material0\" name=\"material0\">" << std::endl;
        fichier << "            <instance_effect url=\"#material0-fx\"/>" << std::endl;
        fichier << "        </material>" << std::endl;
        fichier << "    </library_materials>" << std::endl;
        fichier << "    <library_effects>" << std::endl;
        fichier << "        <effect id=\"material0-fx\">" << std::endl;
        fichier << "            <profile_COMMON>" << std::endl;
        fichier << "                <newparam sid=\"texture0-surface\">" << std::endl;
        fichier << "                    <surface type=\"2D\">" << std::endl;
        fichier << "                        <init_from>texture0</init_from>" << std::endl;
        fichier << "                        <format>R8G8B8</format>" << std::endl;
        fichier << "                    </surface>" << std::endl;
        fichier << "                </newparam>" << std::endl;
        fichier << "                <newparam sid=\"texture0-sampler\">" << std::endl;
        fichier << "                    <sampler2D>" << std::endl;
        fichier << "                        <source>texture0-surface</source>" << std::endl;
        fichier << "                        <minfilter>LINEAR</minfilter>" << std::endl;
        fichier << "                        <magfilter>LINEAR</magfilter>" << std::endl;
        fichier << "                    </sampler2D>" << std::endl;
        fichier << "                </newparam>" << std::endl;
        fichier << "                <technique sid=\"common\">" << std::endl;
        fichier << "                    <blinn>" << std::endl;
        fichier << "                        <emission>" << std::endl;
        fichier << "                            <color>0 0 0 1</color>" << std::endl;
        fichier << "                        </emission>" << std::endl;
        fichier << "                        <ambient>" << std::endl;
        fichier << "                            <color>0 0 0 1</color>" << std::endl;
        fichier << "                        </ambient>" << std::endl;
        fichier << "                        <diffuse>" << std::endl;
        fichier << "                            <texture texture=\"texture0\" texcoord=\"UVSET0\"/>" << std::endl;
        fichier << "                        </diffuse>" << std::endl;
        fichier << "                        <specular>" << std::endl;
        fichier << "                            <color>0 0 0 1</color>" << std::endl;
        fichier << "                        </specular>" << std::endl;
        fichier << "                        <shininess>" << std::endl;
        fichier << "                            <float>0.3</float>" << std::endl;
        fichier << "                        </shininess>" << std::endl;
        fichier << "                        <reflective>" << std::endl;
        fichier << "                            <color>0 0 0 1</color>" << std::endl;
        fichier << "                        </reflective>" << std::endl;
        fichier << "                        <reflectivity>" << std::endl;
        fichier << "                            <float>0.5</float>" << std::endl;
        fichier << "                        </reflectivity>" << std::endl;
        fichier << "                        <transparent>" << std::endl;
        fichier << "                            <color>0 0 0 1</color>" << std::endl;
        fichier << "                        </transparent>" << std::endl;
        fichier << "                        <transparency>" << std::endl;
        fichier << "                            <float>0</float>" << std::endl;
        fichier << "                        </transparency>" << std::endl;
        fichier << "                        <index_of_refraction>" << std::endl;
        fichier << "                            <float>0</float>" << std::endl;
        fichier << "                        </index_of_refraction>" << std::endl;
        fichier << "                    </blinn>" << std::endl;
        fichier << "                </technique>" << std::endl;
        fichier << "            </profile_COMMON>" << std::endl;
        fichier << "        </effect>" << std::endl;
        fichier << "    </library_effects>" << std::endl;
        fichier << "    <library_geometries>" << std::endl;
        fichier << "        <geometry id=\"shape0-lib\" name=\"shape0\">" << std::endl;
        fichier << "            <mesh>" << std::endl;
        fichier << "                <source id=\"shape0-lib-positions\" name=\"position\">" << std::endl;

        fichier << "                    <float_array id=\"shape0-lib-positions-array\" count=\"" << parMesh.vertices.size() * 3 << "\">";
        for (uint i = 0; i < parMesh.vertices.size(); i++)
            fichier << parMesh.vertices[i].x << " " << parMesh.vertices[i].y << " " << parMesh.vertices[i].z << " ";
        fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << parMesh.vertices.size() << "\" source=\"#shape0-lib-positions-array\" stride=\"3\">" << std::endl;
        fichier << "                            <param name=\"X\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Y\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Z\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <source id=\"shape0-lib-normals\" name=\"normal\">" << std::endl;

        fichier << "                    <float_array id=\"shape0-lib-normals-array\" count=\"" << vertexNormals.size() * 3 << "\">";
        for (uint i = 0; i < vertexNormals.size(); i++)
			fichier << vertexNormals[i].x << " " << vertexNormals[i].y << " " << vertexNormals[i].z << " ";
		fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << vertexNormals.size() << "\" source=\"#shape0-lib-normals-array\" stride=\"3\">" << std::endl;
        fichier << "                            <param name=\"X\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Y\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Z\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <source id=\"shape0-lib-map\" name=\"map\">" << std::endl;

        fichier << "                    <float_array id=\"shape0-lib-map-array\" count=\"" << texCoords.size() * 6 << "\">";
        for (uint i = 0; i < texCoords.size(); i++)
        {
            fichier << texCoords[i][0].x / textureSize << " " << 1 - texCoords[i][0].y / textureSize << " " ;
            fichier << texCoords[i][1].x / textureSize << " " << 1 - texCoords[i][1].y / textureSize << " " ;
            fichier << texCoords[i][2].x / textureSize << " " << 1 - texCoords[i][2].y / textureSize << " " ;
        }
        fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << texCoords.size() * 3 << "\" source=\"#shape0-lib-map-array\" stride=\"2\">" << std::endl;
        fichier << "                            <param name=\"U\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"V\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <vertices id=\"shape0-lib-vertices\">" << std::endl;
        fichier << "                    <input semantic=\"POSITION\" source=\"#shape0-lib-positions\"/>" << std::endl;
        fichier << "                </vertices>" << std::endl;
        fichier << "                <triangles count=\"" << parMesh.faces.size() << "\" material=\"material0\">" << std::endl;
        fichier << "                    <input offset=\"0\" semantic=\"VERTEX\" source=\"#shape0-lib-vertices\"/>" << std::endl;
        fichier << "                    <input offset=\"1\" semantic=\"NORMAL\" source=\"#shape0-lib-normals\"/>" << std::endl;
        fichier << "                    <input offset=\"2\" semantic=\"TEXCOORD\" source=\"#shape0-lib-map\"/>" << std::endl;
        fichier << "                    <p>";

        for (size_t i = 0; i < parMesh.faces.size(); i++)
        {
            fichier << parMesh.faces[i].vertices[0] << " " << parMesh.faces[i].vertices[0] << " " << i * 3 << " ";
            fichier << parMesh.faces[i].vertices[1] << " " << parMesh.faces[i].vertices[1] << " " << i * 3 + 1 << " ";
            fichier << parMesh.faces[i].vertices[2] << " " << parMesh.faces[i].vertices[2] << " " << i * 3 + 2 << " ";
        }

        fichier <<"</p>"<< std::endl;
        fichier << "                </triangles>" << std::endl;
        fichier << "            </mesh>" << std::endl;
        fichier << "        </geometry>" << std::endl;
        fichier << "    </library_geometries>" << std::endl;
        fichier << "    <library_visual_scenes>" << std::endl;
        fichier << "        <visual_scene id=\"VisualSceneNode\" name=\"VisualScene\">" << std::endl;
        fichier << "            <node id=\"node\" name=\"node\">" << std::endl;
        fichier << "                <instance_geometry url=\"#shape0-lib\">" << std::endl;
        fichier << "                    <bind_material>" << std::endl;
        fichier << "                        <technique_common>" << std::endl;
        fichier << "                            <instance_material symbol=\"material0\" target=\"#material0\">" << std::endl;
        fichier << "                                <bind_vertex_input semantic=\"UVSET0\" input_semantic=\"TEXCOORD\"/>" << std::endl;
        fichier << "                            </instance_material>" << std::endl;
        fichier << "                        </technique_common>" << std::endl;
        fichier << "                    </bind_material>" << std::endl;
        fichier << "                </instance_geometry>" << std::endl;
        fichier << "            </node>" << std::endl;
        fichier << "        </visual_scene>" << std::endl;
        fichier << "    </library_visual_scenes>" << std::endl;
        fichier << "    <scene>" << std::endl;
        fichier << "        <instance_visual_scene url=\"#VisualSceneNode\"/>" << std::endl;
        fichier << "    </scene>" << std::endl;
        fichier << "</COLLADA>" << std::endl;

        return true;
    }
    return false;
}
// ----------------------------------------------------------------------------
bool SaveMeshToPlyFile(const Mesh& parMesh, char* parFilename, bool parOnlyPos)
{
	assert(parFilename);

	// prepare file
	const char* elementNames[] = {"vertex", "face"};
	float version;
	PlyFile* plyFile = 0;
	int nbElements = (parMesh.faces.size() > 0) ? 2 : 1;
	plyFile = ply_open_for_writing(parFilename, nbElements, elementNames, PLY_ASCII, &version);
	if (!plyFile)
		return false;
	
	// header
	int nbVertices = parMesh.vertices.size();
	assert(nbVertices > 0);
	ply_element_count(plyFile, "vertex", nbVertices);
	int nbVertProps = parOnlyPos ? 3 : 9;
	for (int prop = 0; prop < nbVertProps; ++prop)
		ply_describe_property(plyFile, "vertex", &vertProps[prop]);
	int nbFaces = parMesh.faces.size();
	if (nbFaces > 0)
	{
		ply_element_count(plyFile, "face", nbFaces);
		ply_describe_property(plyFile, "face", &faceProps[0]);
	}
	ply_put_comment(plyFile, "Generated by the RedCloud team softwares");
	ply_header_complete(plyFile);

	// data
	ply_put_element_setup(plyFile, "vertex");
	for (int vertex = 0; vertex < nbVertices; ++vertex)
		ply_put_element(plyFile, (void*) &parMesh.vertices[vertex]);
	if (nbFaces > 0)
	{
		ply_put_element_setup(plyFile, "face");
		for (int face = 0; face < nbFaces; ++face)
			ply_put_element(plyFile, (void*) &parMesh.faces[face]);
	}

	ply_close(plyFile);

	return true;
}
// ----------------------------------------------------------------------------
void DeepCopyMesh(Mesh& parMeshDst, const Mesh& parMeshSrc)
{
	int nbVertices = parMeshSrc.vertices.size();
	assert(nbVertices > 0);
	for (int curVertex = 0; curVertex < nbVertices; ++curVertex)
		parMeshDst.vertices.push_back(parMeshSrc.vertices[curVertex]);

	int nbFaces = parMeshSrc.faces.size();
	for (int curFace = 0; curFace < nbFaces; ++curFace)
	{
		const Face& srcFace = parMeshSrc.faces[curFace];
		assert(srcFace.nbVertices == 3); // only triangles

		Face dstFace;
		dstFace.nbVertices = 3;
		dstFace.vertices = (int*) malloc(sizeof(int) * 3);
		assert(dstFace.vertices);
		dstFace.vertices[0] = srcFace.vertices[0];
		dstFace.vertices[1] = srcFace.vertices[1];
		dstFace.vertices[2] = srcFace.vertices[2];

		parMeshDst.faces.push_back(dstFace);
	}
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
}
// ----------------------------------------------------------------------------
}
// ----------------------------------------------------------------------------
