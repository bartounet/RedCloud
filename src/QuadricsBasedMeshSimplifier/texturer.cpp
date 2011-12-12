
#include "texturer.h"
#include "../common/threedtree.h"
#include <assert.h>
#include <limits>
#include <fstream>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Texturer::Texturer() :
	textureSize_(0)
{
}
// ----------------------------------------------------------------------------
Texturer::Texturer(uint textureSize) :
	textureSize_(textureSize)
{
	assert(textureSize > 0);
	for (uint i = 0; i < textureSize_; i++)
    {
		img_.push_back (std::vector<Com::Color > ());
        for (uint j = 0; j < textureSize_; j++)
        {
			img_[i].push_back (Com::Color (0, 0, 0));
        }
    }
}
// ----------------------------------------------------------------------------
Texturer::~Texturer()
{
}
// ----------------------------------------------------------------------------
void Texturer::CreateMapping(uint nbFaces)
{
	texCoordinates_.reserve(nbFaces);

    triangleSize_ = (uint)((-5 + sqrt(9 + 4 * pow((float)textureSize_, 2) / nbFaces)) / 2);

    int l = 0;
    int c = 0;


    for (uint i = 0; i < nbFaces; i++)
    {
		texCoordinates_.push_back(std::vector<Com::Vec2> ());
        if (i % 2 == 0)
        {
			texCoordinates_[i].push_back(Com::Vec2(0.5 + c * (triangleSize_ + 4), 0.5 + l * (triangleSize_ + 1)));
			texCoordinates_[i].push_back(Com::Vec2(texCoordinates_[i][0].x, texCoordinates_[i][0].y + triangleSize_));
			texCoordinates_[i].push_back(Com::Vec2(texCoordinates_[i][0].x + triangleSize_, texCoordinates_[i][0].y));
        }
        else
        {
			texCoordinates_[i].push_back(Com::Vec2(triangleSize_ + 3.5 + c * (triangleSize_ + 4), triangleSize_ + 0.5 + l * (triangleSize_ + 1)));
			texCoordinates_[i].push_back(Com::Vec2(texCoordinates_[i][0].x, texCoordinates_[i][0].y - triangleSize_));
			texCoordinates_[i].push_back(Com::Vec2(texCoordinates_[i][0].x - triangleSize_, texCoordinates_[i][0].y));

            c++;
            if (((c + 1) * (triangleSize_ + 4)) > textureSize_)
            {
                c = 0;
                l++;
            }
        }

    }
}
// ----------------------------------------------------------------------------
void Texturer::Fill(std::vector<QBMS::Face*>& faces, Com::Mesh pointCloud)
{
	std::vector<Com::Vertex> vertices = pointCloud.vertices;

	std::vector<const Com::Vertex*> verticesPtr;
	for (size_t curVertex = 0; curVertex < vertices.size(); ++curVertex)
		verticesPtr.push_back(&(vertices[curVertex]));
	Com::ThreeDNode<Com::Vertex>* tree = static_cast<Com::ThreeDNode<Com::Vertex>*>( Com::ThreeDNode<Com::Vertex>::BuildTree(verticesPtr, 0, 10) );
	assert(tree);

	uint actualFace = 0;
	for (uint f = 0; f < faces.size (); f++)
    {
		if (faces[f]->IsDegenerated ())
			continue;

		Com::Vec2 vecU = Com::Vec2(texCoordinates_[actualFace][0], texCoordinates_[actualFace][1]);
        Com::Vec2 vecV = Com::Vec2(texCoordinates_[actualFace][0], texCoordinates_[actualFace][2]);

        for (double i = 0; i <= triangleSize_; i += 1)
            for (double j = 0; j <= triangleSize_; j += 1)
            {
                if (i + j <= triangleSize_ + 1)
                {
                    double u = i / triangleSize_;
                    double v = j / triangleSize_;

					double x = texCoordinates_[actualFace][0].x + u * vecU.x + v * vecV.x;
					double y = texCoordinates_[actualFace][0].y + u * vecU.y + v * vecV.y;

					Com::Vec4 origin (faces[f]->V0 ()->Pos ());
					Com::Vec4 vecA (origin, faces[f]->V1 ()->Pos ());
					Com::Vec4 vecB (origin, faces[f]->V2 ()->Pos ());

					Com::Vec4 point (origin.x + u * vecA.x + v * vecB.x,
									 origin.y + u * vecA.y + v * vecB.y,
									 origin.z + u * vecA.z + v * vecB.z,
									 1);

					double dist = std::numeric_limits<double>::max();
					const Com::Vertex* nearestVertex = 0;
					Com::ThreeDNode<Com::Vertex>::NearestPoint(tree, point, 0, dist, &nearestVertex);
					assert(nearestVertex);

					img_[(int)floor(x)][(int)floor(y)] = Com::Color (nearestVertex->r, nearestVertex->g, nearestVertex->b);
                }
            }
		actualFace++;
	}

}
// ----------------------------------------------------------------------------
bool Texturer::DumpTexture(std::string textureName)
{
    std::ofstream fichier(textureName.c_str (), std::ios::out | std::ios::trunc | std::ios::binary);

    if (fichier)
    {
        fichier << "P6" << std::endl;
        fichier << img_.size() << " " << img_[0].size () << std::endl;
        fichier << "255" << ' ';
        for (size_t j = 0; j < img_[0].size (); j++)
            for (size_t i = 0; i < img_.size (); i++)
                fichier << img_[i][j].r << img_[i][j].g << img_[i][j].b;

        fichier.close();
        return true;
    }
    return false;
}
// ----------------------------------------------------------------------------
std::vector<std::vector<Com::Vec2> >& Texturer::GetTexCoords()
{
	return texCoordinates_;
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
