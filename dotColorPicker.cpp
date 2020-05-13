#include "dotColorPicker.h"

dotColorPicker::dotColorPicker(int dotGrid,int dotSize)
{
    spacing = dotGrid;
    size = dotSize;
}

HSLAPixel dotColorPicker::operator()(point p)
{
    int xdist = p.x%spacing;
    int ydist = p.y%spacing;
    int dist1 = (xdist * xdist) + (ydist * ydist);
    int dist2 = ((spacing-xdist) * (spacing-xdist)) + (ydist * ydist);
    int dist3 = ((spacing-ydist) * (spacing-ydist)) + (xdist * xdist);
    int dist4 = ((spacing-xdist) * (spacing-xdist)) + ((spacing-ydist) * (spacing-ydist)) ;
    int ss = size*size;
    if (dist1<= ss || dist2<= ss || dist3<=ss || dist4 <=ss){
        return p.c.color;
    } else {
        HSLAPixel ret;
        ret.h = p.c.color.h; ret.s = p.c.color.s ; ret.l = ( 7. * p.c.color.l)/8.;
        return ret;
    }

}
