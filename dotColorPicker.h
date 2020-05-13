/**
 * @file dotColorPicker.h
 * Definition of a dot color picker.
 *
 */
#ifndef DOTCOLORPICKER_H
#define DOTCOLORPICKER_H

#include "colorPicker.h"
#include "point.h"
#include "center.h"

/**
 * dotColorPicker: a functor that determines the color that should be used
 * given an x and a y coordinate using a pattern of dots on a grid.
 *
 */
class dotColorPicker : public colorPicker
{
  public:
    /**
     * Constructs a new dotColorPicker.
     *
     * @param fillColor Color for the dots.
     * @param dotGrid the distance between dot centers.
     * @param dotSize the radius of the dots.
     */
    dotColorPicker(int dotGrid, int dotSize);

    /**
     * Picks the color for pixel (x, y). 
    * If the distance from point (x,y) to a grid point is less than 
    * dotSize, then return fillColor,
     * otherwise, it return white.
    *
    * Grid points are the points where x == y == 0 mod dotGrid. 
    * Distances between pixel locations are calculated using a 
    * Euclidian metric, but because we don't like computing square
    * roots, we compare squared distances. Based on these 
    * observations you should be able to devise a fairly simple
    * computation to use to determine if an (x,y) point is within
    * a dot. Note that if dotSize is a little more than dotGrid, the
    * entire fill region will be colored with the dot color.
     *
     * @param x The x coordinate to pick a color for.
     * @param y The y coordinat to pick a color for.
     * @return The color chosen for (x, y).
     */
    virtual HSLAPixel operator()(point p);

  private:

    int spacing;     /**< spacing on which dots are centered. */
    int size;     /**< size of the dots */
};

#endif
