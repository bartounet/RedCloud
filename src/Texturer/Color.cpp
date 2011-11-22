#include "Color.h"

/** @brief Color
  *
  * @todo: document this function
  */

Color::Color(unsigned char r, unsigned char g, unsigned char b)
    : r_(r),
      g_(g),
      b_(b)
{
}

std::ostream& operator<<( std::ostream &flux, Color* const& c)
{
    flux << "(" << c->r_ << ", " << c->g_ << ", "  << c->b_ << ")";
    return flux;
}
