#ifndef TEXTURER_H_
#define TEXTURER_H_


#include <string>
#include <vector>
#include "../common/types.h"
#include "face.h"

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Texturer
{
public:
	Texturer();
	Texturer(uint textureSize);
	~Texturer();

public:
	void CreateMapping(uint nbFaces);
	void Fill(std::vector<QBMS::Face*>& faces, Com::Mesh pointCloud);
	bool DumpTexture(std::string textureName);
	std::vector<std::vector<Com::Vec2> >& GetTexCoords();

private:
	uint textureSize_;
	uint triangleSize_;
	uint nbFaces_;
	std::vector<std::vector<Com::Vec2> > texCoordinates_;

	std::vector<std::vector<Com::Color > > img_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 