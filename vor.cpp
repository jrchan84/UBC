/**
 * @file vor.cpp
 * Implementation of functions in the filler namespace.
 *
 */
//included within  "vor.h" for template instantiation


/* vorDotDFS given as an example for PA2 */
animation filler::vorDotDFS(PNG& img, double density,
                                int dotGrid,int dotSize, int frameFreq)
{
    dotColorPicker a(dotGrid,dotSize);
    return vor<Stack>(img, density, a, frameFreq);
}

animation filler::vorSolidDFS(PNG& img, double density, int frameFreq)
{
    solidColorPicker a;
    return vor<Stack>(img, density, a, frameFreq);

}

animation filler::vorFadeDFS(PNG& img, double density, double fadeFactor, int frameFreq)
{
    fadeColorPicker a(fadeFactor);
    return vor<Stack>(img, density, a, frameFreq);

}

// animation filler:: vorCustomDFS(PNG& img, double density, double fadeFactor, int frameFreq)
// {
//     customColorPicker a(fadeFactor);
//     return vor<Stack>(img, density, a, frameFreq);
// }


/* vorDotBFS given as an example for PA2 */
animation filler::vorDotBFS(PNG& img, double density,
                                int dotGrid,int dotSize, int frameFreq)
{
    dotColorPicker a(dotGrid, dotSize);
    return vor<Queue>(img, density, a, frameFreq);
}

animation filler::vorSolidBFS(PNG& img, double density, int frameFreq)
{
    solidColorPicker a;
    return vor<Queue>(img, density, a, frameFreq);
}

animation filler::vorFadeBFS(PNG& img, double density, double fadeFactor, int frameFreq)
{
    fadeColorPicker a(fadeFactor);
    return vor<Queue>(img, density, a, frameFreq);
}

// animation filler:: vorCustomBFS(PNG& img, double density, double fadeFactor, int frameFreq)
// {
//   customColorPicker a(fadeFactor);
//   return vor<Queue>(img, density, a, frameFreq);
//
// }


template <template <class T> class OrderingStructure>
animation filler::vor(PNG& img, double density, colorPicker& fillColor,
                       int frameFreq)
{
     animation animation;
     int frame = 0;
     int width = img.width();
     int height = img.height();
     // * @todo You need to implement this function!
     // *
     // * This is a general description of a space filling algorithm, where
     // * the space is partitioned into a set of regions each color of which
     // * is determined by the color of a point
     // * chosen from an original image. We call those original points
     // * "centers." (See pictures in the specification.)
     // *
     // * The algorithm requires ordering structures used to orchestrate
     // * the sequence of points filled. This structure type info is passed to the
     // * fill algorithm via its template parameter. For a breadth-first fill,
     // * we use Queues, and for a depth-first fill, we use Stacks. Function
     // * vorDotDFS provides an example call to function vor.
     // *
     // * The algorithm includes the following steps:
     // * 1) you will first select a random set
     // * of coordinates (and their colors) to use as centers from which to
     // * begin the fill. (this function is in file vor_given.cpp -- for
     // * testing purposes, you'll use regularly spaced points, rather than
     // * random.)

     vector<center> centers = randSample(img, density);

    //  * 2) Initialize a structure to use to maintain a collection of
    //  * ordering structures

    vector<OrderingStructure<point>> queue;
    vector<vector<bool>> processed;

    for(int i = 0; i < width; i++) {
      vector<bool> osv;
      for(int j = 0; j < height; j++) {
        osv.push_back(false);
      }
      processed.push_back(osv);
    }


    PNG newImg = PNG(width, height);

    //  *
    //  * 3) Initialize the fill: For each center c, in round-robin order,
    //  *     do the following:
    //  *     a)  add the center to the its ordering structure. (every center
    //  *         its own!
    //  *     b)  mark the location of that center as processed (the way you
    //  *         do this is up to you!)
    //  *     c)  we have a choice to actually set the color of the pixel in
    //  *         the fill when we add or when we remove. In our test cases,
    //  *         we assume that you will change a color when a point is
    //  *         added to a structure. (this is mostly relevant for checking
    //  *         the animations.)

    int centerSize = centers.size();

    vector<center> temp;
    while (!centers.empty()) {
      temp.push_back(centers.back());
      centers.pop_back();
    }

    for (int i=0; i < centerSize; i++){
      point p = point(temp.back());
      temp.pop_back();

      OrderingStructure<point> os;               //a
      os.add(p);
      queue.push_back(os);

      processed[p.x][p.y] = true;                //b

      process(p, newImg, fillColor, frame, animation, frameFreq);             //c
    }

    cout << "finish centers" << endl;


    // at this point, centers is empty, and queue is full of ordering structures
    // each with a point in them, the center.

    //  *
    //  * 5) Complete the fill: Until all ordering structures (OS) are empty,
    //  *    do the following for each OS, in a round robin order, indexing
    //  *    the iterations by k (an integer distance from the center):
    //  *
    //  *      For each point less than or equal to distance k from its center:
    //  *      a)Remove the  point p from the ordering structure, and then...
    //  *
    //  *        i.    add p's unprocessed neighbors to the ordering structure if
    //  *              they are within the appropriate distance from the center. We
    //  *              describe this more carefully below. We call these the "valid"
    //  *              neighbors.
    //  *        ii.    use the colorPicker to set the new color of the valid neighbors.
    //  *        iii.    mark the valid neighbors as processed.
    //  *        iv.    as each neighbor is processed, if it is an appropriate
    //  *              frame, send the current PNG to the
    //  *              animation (as described below).

    //  *      b) When implementing your breadth-first and depth-first fills,
    //  *            you will need to explore neighboring pixels in some order.
    //  *
    //  *        For this assignment, each pixel p has *up to* 8 neighbors, consisting of
    //  *        the 8 pixels who share an edge with p. (We leave it to
    //  *        you to describe those 8 pixel locations, relative to the location
    //  *        of p.)
    //  *
    //  *        While the order in which you examine neighbors does not matter
    //  *        for a proper fill, you must use the same order as we do for
    //  *        your animations to come out like ours!
    //  *
    //  *        The order you should put
    //  *        neighboring pixels **ONTO** the queue or stack is as follows:
    //  *        ** UP(-y), UPLEFT(-x,-y), LEFT(-x), LEFTDOWN(-x,+y), DOWN(+y),
    //  *        DOWNRIGHT(+x,+y), RIGHT(+x), RIGHTUP(+x,-y)**
    //  *
    //  *        If you do them in a different order, your fill may
    //  *        still work correctly, but your animations will be different
    //  *        from the grading scripts!
    //  *
    //  *        TWO IMPORTANT NOTES:
    //  *        1) *UP* here means towards the top of the image, so since an image has
    //  *        smaller y coordinates at the top, this is in the *negative y*
    //  *        direction. Similarly, *DOWN* means in the *positive y*
    //  *        direction.


    //  *        2) not all of the 8 neighbors will be processed at every iteration of the
    //  *        while loop. You must assure that all vertices distance k or less
    //  *        from the center are processed (colored and put on the queue) before
    //  *        ANY whose distance is *greater than* k. k is a non-negative integer.
    //  *        Finally, distance is the standard Euclidean distance  (computed
    //  *        using the Pythagorean theorem). It is this requirement that assures
    //  *        that the fill grows outward from the center in the shape of a circle,
    //  *        rather than a diamond or a square.


    //  *      c) For every g pixels filled, **starting at the gth pixel**, you
    //  *        must add a frame to the animation, where g = frameFreq.
    //  *
    //  *        For example, if frameFreq is 4, then after the 4th pixel has
    //  *        been filled you should add a frame to the animation, then again
    //  *        after the 8th pixel, etc.  You must only add frames for the
    //  *        number of pixels that have been filled, not the number that
    //  *        have been checked. So if frameFreq is set to 1, a pixel should
    //  *        be filled every frame.


    //  *      d) Finally, as you leave the function, send one last frame to the
    //  *        animation. This frame will be the final result of the fill, and
    //  *        it will be the one we test against.

     /* Your code here. As a point of reference, we used three different helper
      * functions to complete this code. You may add as many as you wish, since
      * we will be grading vor.h. File "vor_given.cpp also includes the function
      * used to generate the original set of centers.
      */

    int k = 0;
    int dist = 0;
    int queueSize = queue.size();

    if (!queue.empty()){
      // int counter = 0;

      // while (counter!=queue.size()) {
        for (int i=0; i<queueSize; i++) {
          cout << i << endl;
          point p = queue[i].remove();
          k = p.level;

          addNeighbors(p, newImg, processed, queue[i]);

          OrderingStructure<point> temp;
          while(!queue[i].isEmpty()) {
            cout << "while" << i << endl;
            point p = queue[i].remove();
            dist = pow(p.c.x-p.x,2) + pow(p.c.y-p.y, 2);
            if(sqrt(dist)<= k+1){
              process(p, newImg, fillColor, frame, animation, frameFreq);
              processed[p.x][p.y] = true;
            } else {
              temp.add(p);
            }
          }

          while(!temp.isEmpty()) {
            queue[i].add(temp.peek());
            temp.remove();
          }

          if (i == queueSize-1) {
            if (!isOver(queue)) {
              i = 0;
            }
          }

          // if(queue[i].isEmpty()) {
          //   queue.erase(i);
          // }

          // for (int j = 0; j < neighborSize;j++) {
          //   dist = pow(neighbors[j].c.x-neighbors[j].x,2) + pow(neighbors[j].c.y-neighbors[j].y, 2);
          //   if (sqrt(dist) <= k+1) {
          //     process(neighbors[j], newImg, fillColor, frame, animation, frameFreq);
          //     processed[neighbors[j].x][neighbors[k].y] = true;
          //   }
          // }

          // if (!queue.empty()) {
          //   bool counter = false;
          //   for (int i = 0; i<queueSize; i++) {
          //     bool empty = queue[i].isEmpty();
          //     if (!empty) {
          //       counter = true;
          //     }
          //   }
          //   if (counter) {
          //     i = 0;
          //   }
          // }
        }
    //     counter = 0;
    //     for (OrderingStructure<point> s : queue) {
    //       if (s.isEmpty()) {
    //         counter++;
    //       }
    //     }
    // }
  }

    animation.addFrame(newImg); // regardless of frameFreq, last frame is sent.
    return animation;
}

// HELPER FUNCTIONS
template <template <class T> class OrderingStructure>
bool filler::isOver(vector<OrderingStructure<point>> & structure) {
  for (int i=0; i< int(structure.size()); i++) {
    if (!structure[i].isEmpty()) {
      return false;
    }
    return true;
  }
}

void filler::sendFrame(animation& ai, PNG & img, int frame, int frameFreq) {
  if((frame % frameFreq) == 0) {
    ai.addFrame(img);
  }
}

bool filler::inBounds(point p, PNG &img) {
  return p.x >= 0 && p.x<img.width() && p.y >= 0 && p.y < img.height();
}

void filler::process(point p, PNG& newImg, colorPicker& fillColor, int& frame, animation & animation, int frameFreq) {
  HSLAPixel color = fillColor(p);
  HSLAPixel* pixel = newImg.getPixel(p.x,p.y);
  *pixel = color;
  frame++;
  sendFrame(animation, newImg, frame, frameFreq);
}

void filler::addNeighbors(point p, PNG &newImg, vector<vector<bool>> & processed, OrderingStructure<point> & queue) {
  point up = point(p.x, p.y-1, p.c, p.level+1);
  if(inBounds(up,newImg) && !processed[up.x][up.y]) {
    queue.add(up);
  }

  point upleft = point(p.x-1, p.y-1, p.c, p.level+1);
  if(inBounds(upleft,newImg) && !processed[upleft.x][upleft.y]) {
    queue.add(upleft);
  }

  point left = point(p.x-1, p.y, p.c, p.level+1);
  if(inBounds(left,newImg) && !processed[left.x][left.y]) {
    queue.add(left);
  }

  point downleft = point(p.x-1, p.y+1, p.c, p.level+1);
  if(inBounds(downleft,newImg) && !processed[downleft.x][downleft.y]) {
    queue.add(downleft);
  }

  point down = point(p.x, p.y+1, p.c, p.level+1);
  if(inBounds(down,newImg) && !processed[down.x][down.y]) {
    queue.add(down);
  }

  point downright = point(p.x+1, p.y+1, p.c, p.level+1);
  if(inBounds(downright,newImg) && !processed[downright.x][downright.y]) {
    queue.add(downright);
  }

  point right = point(p.x+1, p.y, p.c, p.level+1);
  if(inBounds(right,newImg) && !processed[right.x][right.y]) {
    queue.add(right);
  }

  point upright = point(p.x+1, p.y-1, p.c, p.level+1);
  if(inBounds(upright,newImg) && !processed[upright.x][upright.y]) {
    queue.add(upright);
  }
}
