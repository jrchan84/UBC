#ifndef _CENTER_H
#define _CENTER_H

#include "cs221util/HSLAPixel.h"
using namespace cs221util;


/* A collection of centers is used to initiate the fill. Ideally, these will
 * be randomly selected so we achieve an artistic effect, but we'll also 
 * build deterministic sets of centers for testing.
 * 
 * A center consists of a location on an image, together with a color.
*/
struct center{

    int x;
    int y;
    HSLAPixel color;

    center(int newx, int newy, HSLAPixel c){ x = newx; y = newy; color = c;}
    center(){ x = 0; y = 0; color = HSLAPixel(240,1.,0.5);} // default color is blue

};

#endif
