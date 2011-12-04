#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED


#include <cstddef>
#include <vector>
#include <string>
#include "Face.h"
#include "Color.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
class Texture
{
public:
    Texture(size_t w, std::vector<Face* >& faces);

public:
    void createMapping ();
    void fill ();
    bool dump (std::string filename);

public:
    size_t getSizeTriangle ();

private:
    size_t width_;
    size_t heigth_;
    size_t sizeTriangle_;
    std::vector<Face* >& faces_;
    std::vector<std::vector<Color > > img_;
};
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif // TEXTURE_H_INCLUDED
