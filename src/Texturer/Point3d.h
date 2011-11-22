#ifndef POINT3D_H_INCLUDED
#define POINT3D_H_INCLUDED

#include <iostream>

struct Point3d
{
  float x_;
  float y_;
  float z_;

  Point3d (float x, float y, float z);
};

std::ostream& operator<<( std::ostream &flux, Point3d* const& p);

#endif // POINT3D_H_INCLUDED
