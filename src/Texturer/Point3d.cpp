
#include "Point3d.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
Point3d::Point3d(float x, float y, float z)
     : x_(x),
       y_(y),
       z_(z)
{

}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
std::ostream& operator<<( std::ostream &flux, Point3d* const& p)
{
    flux << "[" << p->x_ << ", " << p->y_ << ", " << p->z_ << "]" ;
    return flux;
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================