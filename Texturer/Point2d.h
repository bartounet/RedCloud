#ifndef POINT2D_H_INCLUDED
#define POINT2D_H_INCLUDED

#include <iostream>

struct Point2d
{
  float x_;
  float y_;

  Point2d (float x, float y);
};

std::ostream& operator<<( std::ostream &flux, Point2d* const& p);

#endif // POINT2D_H_INCLUDED
