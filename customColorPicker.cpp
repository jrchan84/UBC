#include "customColorPicker.h"

customColorPicker::customColorPicker(double fade)
{
    fadeFactor = fade;
    count = 1;
}

HSLAPixel customColorPicker::operator()(point p)
{
  double distX = p.x - p.c.x;
  double distY = p.y - p.c.y;

  double distance = sqrt(pow(distX, 2) + pow(distY, 2));

  double newL = p.c.color.l * pow(fadeFactor, distance);

  double newA = color(p.c.color.a);

  return HSLAPixel(newA, p.c.color.s, newL, p.c.color.a);
}

double customColorPicker::color(double a)
{
  double c = (count + a);
  count += 10;
  if (c > 360) {
    return c % 360;
  } else {
    return c;
  }
}
