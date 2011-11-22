#include "Point2dColor.h"

/** @brief Point2dColor
  *
  * @todo: document this function
  */
 Point2dColor::Point2dColor(float x, float y, Color c)
    : Point2d(x, y),
      color_(Color(c.r_, c.g_, c.b_))
{
}


std::ostream& operator<<( std::ostream &flux, Point2dColor* const& p)
{
    flux << "[" << p->x_ << ", " << p->y_ << "] " << &p->color_  ;
    return flux;
}
