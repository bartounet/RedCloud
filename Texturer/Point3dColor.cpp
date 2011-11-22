#include "Point3dColor.h"

/** @brief Point3dColor
  *
  * @todo: document this function
  */
 Point3dColor::Point3dColor(float x, float y, float z, Color c)
    : Point3d(x, y, z),
      color_(Color(c.r_, c.g_, c.b_))
{
}

