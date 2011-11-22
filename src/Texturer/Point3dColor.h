#ifndef POINT3DCOLOR_H_INCLUDED
#define POINT3DCOLOR_H_INCLUDED

#include "Point3d.h"
#include "Color.h"

struct Point3dColor : public Point3d
{
  Color color_;

  Point3dColor (float x, float y, float z, Color c);
};

#endif // POINT3DCOLOR_H_INCLUDED
