#ifndef PLAN_H_INCLUDED
#define PLAN_H_INCLUDED

#include <iostream>

struct Plan
{
    float a_;
    float b_;
    float c_;
    float d_;

    Plan (float a, float b, float c, float d);
};

std::ostream& operator<<( std::ostream &flux, Plan* const& p);

#endif // PLAN_H_INCLUDED
