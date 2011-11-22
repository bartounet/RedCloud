#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <cstddef>
#include <vector>
#include <string>
#include "Face.h"
#include "Color.h"


class Texture
{
  private:
    size_t width_;
    size_t heigth_;
    size_t sizeTriangle_;
    std::vector<Face* >& faces_;
    std::vector<std::vector<Color > > img_;

  public:
    Texture (size_t w, std::vector<Face* >& faces);
    void createMapping ();
    void fill ();
    bool dump (std::string filename);
    size_t getSizeTriangle ();
};


#endif // TEXTURE_H_INCLUDED
