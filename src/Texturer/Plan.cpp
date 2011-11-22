#include "Plan.h"

/** @brief Plan
  *
  * @todo: document this function
  */
 Plan::Plan(float a, float b, float c, float d)
    : a_(a),
      b_(b),
      c_(c),
      d_(d)
{
}

std::ostream& operator<<( std::ostream &flux, Plan* const& p)
{
    flux << "plan[" << p->a_ << ", " << p->b_ << ", " << p->c_ << ", " << p->d_ << "]";
    return flux;
}
