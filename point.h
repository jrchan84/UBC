#ifndef _POINT_H
#define _POINT_H

#include "center.h"

/* Every point in the fill is associated with a location on the image,
 * and with a center. The center information is instantiated during the
 * fill. This is necessary because we need to pass center color information
 * to the color picker.
*/

struct point{

    int x;
    int y;
    
    center c;
    int level;

    point(center ctr){ level = 0; c = ctr; x = ctr.x; y = ctr.y; }
    point(int newx, int newy, center ctr, int l){ level = l; c = ctr; x = newx; y = newy; }

};

#endif
