#include "solidColorPicker.h"

solidColorPicker::solidColorPicker()
{
/* your code here  :)  */
}

HSLAPixel solidColorPicker::operator()(point p)
{
    return p.c.color;
}
