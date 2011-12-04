#ifndef POINT2D_H_INCLUDED
#define POINT2D_H_INCLUDED

#include <iostream>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
struct Point2d
{
  float x_;
  float y_;

  Point2d (float parX, float parY);
};
// ----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream &parFlux, Point2d* const& parPoint2d);
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif // POINT2D_H_INCLUDED
