#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
  double distX = p.x - p.c.x;
  double distY = p.y - p.c.y;

  double distance = sqrt(pow(distX, 2) + pow(distY, 2));

  double newL = p.c.color.l * pow(fadeFactor, distance);

  return HSLAPixel(p.c.color.h, p.c.color.s, newL, p.c.color.a);
}
