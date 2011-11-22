#include <iostream>
#include <limits>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "Point2d.h"
#include "Point3d.h"
#include "Point2dColor.h"
#include "Point3dColor.h"
#include "Face.h"
#include "Plan.h"
#include "World.h"
#include "Texture.h"

#include "mesh_file_helper.h"

bool load_point_cloud (std::string filename, std::vector<Point3dColor* >& points, Point3d& min, Point3d& max)
{
    char* cfilename = (char*) malloc (sizeof (char) * filename.size () + 1);
    strncpy (cfilename, filename.c_str (), filename.size ());
    cfilename[filename.size ()] = '\0';

    VR::Mesh mesh;
	bool result = VR::MeshFileHelper::LoadMeshFromPlyFile(mesh, cfilename);
	if (!result)
	{
		printf("[-] Failed to load triangularized mesh '%s'\n", filename.c_str());
		exit(1);
	}
	std::cout << mesh.vertices.size () << std::endl;

    for (size_t i = 0; i < mesh.vertices.size (); ++i)
    {
        points.push_back (new Point3dColor(mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z,
                                           Color(mesh.vertices[i].r, mesh.vertices[i].g, mesh.vertices[i].b)));

        min.x_ = std::min (min.x_, mesh.vertices[i].x);
        min.y_ = std::min (min.y_, mesh.vertices[i].y);
        min.z_ = std::min (min.z_, mesh.vertices[i].z);
        max.x_ = std::max (max.x_, mesh.vertices[i].x);
        max.y_ = std::max (max.y_, mesh.vertices[i].y);
        max.z_ = std::max (max.z_, mesh.vertices[i].z);
    }

    return result;
}

bool load_faces (std::string filename, std::vector<Face* >& faces, std::vector<Point3dColor* >& facePoints, Point3d& min, Point3d& max)
{
    char* cfilename = (char*) malloc (sizeof (char) * filename.size () + 1);
    strncpy (cfilename, filename.c_str (), filename.size ());
    cfilename[filename.size ()] = '\0';

    VR::Mesh mesh;
	bool result = VR::MeshFileHelper::LoadMeshFromPlyFile(mesh, cfilename);
	if (!result)
	{
		printf("[-] Failed to load triangularized mesh '%s'\n", filename.c_str());
		exit(1);
	}
	std::cout << mesh.vertices.size () << std::endl;

    for (size_t i = 0; i < mesh.vertices.size (); ++i)
    {
        facePoints.push_back (new Point3dColor(mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z,
                                               Color(mesh.vertices[i].r, mesh.vertices[i].g, mesh.vertices[i].b)));

        min.x_ = std::min (min.x_, mesh.vertices[i].x);
        min.y_ = std::min (min.y_, mesh.vertices[i].y);
        min.z_ = std::min (min.z_, mesh.vertices[i].z);
        max.x_ = std::max (max.x_, mesh.vertices[i].x);
        max.y_ = std::max (max.y_, mesh.vertices[i].y);
        max.z_ = std::max (max.z_, mesh.vertices[i].z);
    }

    std::cout << "faces points loaded : " << facePoints.size() << std::endl;

    for (size_t i = 0; i < mesh.faces.size (); ++i)
    {
        assert (mesh.faces[i].nbVertices == 3);
        faces.push_back (new Face (mesh.faces[i].vertices[0], mesh.faces[i].vertices[1], mesh.faces[i].vertices[2]));
        //faces.push_back (new Face (0, 0, 0));
    }

    std::cout << "faces loaded : " << faces.size () << std::endl;

    std::cout << "min : " << &min << std::endl;
    std::cout << "max : " << &max << std::endl;


    return true;
}

bool write_ply (std::string filename, std::vector<Face* >& faces, std::vector<Point3dColor* >& facePoints, size_t sizeTriangle, size_t sizeTexture)
{
    std::ofstream fichier(filename.c_str (), std::ios::out | std::ios::trunc);

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

        fichier << "                    <float_array id=\"shape0-lib-positions-array\" count=\"" << facePoints.size () * 3 << "\">";
        for (size_t i = 0; i < facePoints.size (); i++)
            fichier << facePoints[i]->x_ << " " << facePoints[i]->y_ << " " << facePoints[i]->z_ << " ";
        fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << facePoints.size () << "\" source=\"#shape0-lib-positions-array\" stride=\"3\">" << std::endl;
        fichier << "                            <param name=\"X\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Y\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Z\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <source id=\"shape0-lib-normals\" name=\"normal\">" << std::endl;

        fichier << "                    <float_array id=\"shape0-lib-normals-array\" count=\"" << faces.size () * 3 << "\">";
        for (size_t i = 0; i < faces.size (); i++)
            fichier << faces[i]->getPlan ().a_ << " " << faces[i]->getPlan ().b_ << " " << faces[i]->getPlan ().c_ << " ";
        fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << faces.size () << "\" source=\"#shape0-lib-normals-array\" stride=\"3\">" << std::endl;
        fichier << "                            <param name=\"X\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Y\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"Z\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <source id=\"shape0-lib-map\" name=\"map\">" << std::endl;

        fichier << "                    <float_array id=\"shape0-lib-map-array\" count=\"" << faces.size () * 6 << "\">";
        for (size_t i = 0; i < faces.size (); i++)
        {
            Point2d* p0 = faces[i]->getCoordTexture ()[0];
            Point2d* p1 = faces[i]->getCoordTexture ()[1];
            Point2d* p2 = faces[i]->getCoordTexture ()[2];

            /*Point2d vectA = Point2d ((p1->x_ - p0->x_) / sizeTriangle, (p1->y_ - p0->y_) / sizeTriangle);
            Point2d vectB = Point2d ((p2->x_ - p0->x_) / sizeTriangle, (p2->y_ - p0->y_) / sizeTriangle);

            assert (vectA.x_ == 0 || vectA.y_ == 0);
            assert (vectB.x_ == 0 || vectB.y_ == 0);
            assert (abs(vectA.x_) == 1 || abs(vectA.y_) == 1);
            assert (abs(vectB.x_) == 1 || abs(vectB.y_) == 1);


            Point2d np0 = Point2d (p0->x_ + vectA.x_ + vectB.x_, p0->y_ + vectA.y_ + vectB.y_);
            Point2d np1 = Point2d (p1->x_ - vectA.x_ + vectB.x_, p1->y_ - vectA.y_ + vectB.y_);
            Point2d np2 = Point2d (p2->x_ + vectA.x_ - vectB.x_, p2->y_ + vectA.y_ - vectB.y_);

            fichier << np0.x_ / sizeTexture << " " << 1 - np0.y_ / sizeTexture << " " ;
            fichier << np1.x_ / sizeTexture << " " << 1 - np1.y_ / sizeTexture << " " ;
            fichier << np2.x_ / sizeTexture << " " << 1 - np2.y_ / sizeTexture << " " ;
            */
            fichier << p0->x_ / sizeTexture << " " << 1 - p0->y_ / sizeTexture << " " ;
            fichier << p1->x_ / sizeTexture << " " << 1 - p1->y_ / sizeTexture << " " ;
            fichier << p2->x_ / sizeTexture << " " << 1 - p2->y_ / sizeTexture << " " ;
        }
        fichier << "</float_array>" << std::endl;

        fichier << "                    <technique_common>" << std::endl;
        fichier << "                        <accessor count=\"" << faces.size () * 3 << "\" source=\"#shape0-lib-map-array\" stride=\"2\">" << std::endl;
        fichier << "                            <param name=\"U\" type=\"float\"/>" << std::endl;
        fichier << "                            <param name=\"V\" type=\"float\"/>" << std::endl;
        fichier << "                        </accessor>" << std::endl;
        fichier << "                    </technique_common>" << std::endl;
        fichier << "                </source>" << std::endl;
        fichier << "                <vertices id=\"shape0-lib-vertices\">" << std::endl;
        fichier << "                    <input semantic=\"POSITION\" source=\"#shape0-lib-positions\"/>" << std::endl;
        fichier << "                </vertices>" << std::endl;
        fichier << "                <triangles count=\"" << faces.size () << "\" material=\"material0\">" << std::endl;
        fichier << "                    <input offset=\"0\" semantic=\"VERTEX\" source=\"#shape0-lib-vertices\"/>" << std::endl;
        fichier << "                    <input offset=\"1\" semantic=\"NORMAL\" source=\"#shape0-lib-normals\"/>" << std::endl;
        fichier << "                    <input offset=\"2\" semantic=\"TEXCOORD\" source=\"#shape0-lib-map\"/>" << std::endl;
        fichier << "                    <p>";

        for (size_t i = 0; i < faces.size (); i++)
        {
            fichier << faces[i]->p0_ << " " << i << " " << i * 3 << " ";
            fichier << faces[i]->p1_ << " " << i << " " << i * 3 + 1 << " ";
            fichier << faces[i]->p2_ << " " << i << " " << i * 3 + 2 << " ";
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

int main (int argc, char** argv)
{
    assert (argc == 4);

    Point3d min = Point3d (std::numeric_limits<float>::max (), std::numeric_limits<float>::max (), std::numeric_limits<float>::max ());
    Point3d max = Point3d (std::numeric_limits<float>::min (), std::numeric_limits<float>::min (), std::numeric_limits<float>::min ());
    std::vector<Point3dColor* > colorPoints;
    std::vector<Face* > faces;

    load_point_cloud(argv[1], colorPoints, min, max);
    load_faces (argv[2], faces, Face::points_, min, max);

    min.x_--;
    min.y_--;
    min.z_--;
    max.x_++;
    max.y_++;
    max.z_++;



    std::cout << "begin" << std::endl;
    World world = World (colorPoints, faces, min, max);
    std::cout << "1" << std::endl;
    world.associatePoints();
    Face::printInfos (faces);
    std::cout << "2" << std::endl;

    size_t sizeTexture = 8;

    Texture texture = Texture (sizeTexture, faces);
    std::cout << "3" << std::endl;
    texture.createMapping ();

    std::cout << "4" << std::endl;
    texture.fill ();
    std::cout << "5" << std::endl;
    texture.dump ("texture.ppm");

    write_ply (argv[3], faces, Face::points_, texture.getSizeTriangle (), sizeTexture);

    std::cout << "end" << std::endl;
    return 0;
}
