#ifndef POINT2DCOLOR_H_INCLUDED
#define POINT2DCOLOR_H_INCLUDED
#include "Color.h"
#include "Point2d.h"

struct Point2dColor : public Point2d
{
  Color color_;

  Point2dColor (float x, float y, Color c);
};

std::ostream& operator<<( std::ostream &flux, Point2dColor* const& p);


#endif // POINT2DCOLOR_H_INCLUDED
