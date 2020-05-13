#include "block.h"
#include "cs221util/HSLAPixel.h"
#include <vector>

/**
 * Returns the width of the block.
 */
int Block::width() const{
  return Block::data.size();
}

/**
 * Returns the height of the block.
 */
int Block::height() const{
  return Block::data[0].size();
}

/**
 * Default Block constructor.
 */
Block::Block() {/* nothing */}

/**
 * Useful Block constructor.
 * Makes a block from the rectangle of width by height pixels in im
 * whose upper-left corner is at position (x,y).
 */
Block::Block(PNG & im, int x, int y, int width, int height) {
  vector<vector<HSLAPixel>> vecx;
  for (int x1 = 0; x1 < width; x1++) {
    vector<HSLAPixel> vecy;
    for (int y1 = 0; y1 < height; y1++) {
      HSLAPixel* pixel = im.getPixel(x+x1, y+y1);
      vecy.push_back(*pixel);
    }
    vecx.push_back(vecy);
  }
  data = vecx;
}

/**
 * Draws the block at position (x,y) in the image im
 */
void Block::render(PNG & im, int x, int y) const {
  for (int x1 = 0; x1 < width(); x1++) {
    for (int y1 = 0; y1 < height(); y1++) {
      HSLAPixel* pixel = im.getPixel(x+x1,y+y1);
      *pixel = data[x1][y1];
    }
  }
}

/**
 * Changes the saturation of every pixel in the block to 0,
 * which removes the color, leaving grey.
 */
void Block::greyscale() {
  for (int x = 0; x < width(); x++) {
    for (int y = 0; y < height(); y++) {
      HSLAPixel* pixel = &data[x][y];
      pixel->s = 0;
    }
  }
}
