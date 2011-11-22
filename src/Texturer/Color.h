#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <iostream>

struct Color
{
    unsigned char r_;
    unsigned char g_;
    unsigned char b_;
    Color (unsigned char r, unsigned char g, unsigned char b);

};

std::ostream& operator<<(std::ostream &flux, Color* const& c);


#endif // COLOR_H_INCLUDED
