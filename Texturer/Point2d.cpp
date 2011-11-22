#include "Point2d.h"
/** @brief Point2d
  *
  * @todo: document this function
  */
 Point2d::Point2d(float x, float y)
    : x_(x),
      y_(y)
{
}

std::ostream& operator<<( std::ostream &flux, Point2d* const& p)
{
    flux << "[" << p->x_ << ", " << p->y_ << "]" ;
    return flux;
}
