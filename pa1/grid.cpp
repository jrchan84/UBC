#include "grid.h"
#include "grid_given.cpp"

// PA1 functions


/**
 * Destroys the current Grid. This function should ensure that
 * memory does not leak on destruction of a grid.
 */
Grid::~Grid(){ /*your code here*/
  clear();
}

/**
 * Rotate row r (row 0 is the first) by count positions.
 * If row r is ABCDE (left to right) in grid g then row r
 * in grid g should be DEABC after the call g.rotateR(r, 2).
 * Rotate headOfCol_ if necessary.
 */
void Grid::rotateR(int r, int count) {
  r = r % numRows();

  if (count == 0 || count < 0) {
    return;
  }

  for (int i=0; i < count; i++) { // loops for each time shifted
    Node *b = headOfRow_[r];
    headOfRow_[r] = b->left;  // move headOfRow_[r] left

    for(int j=0; j < numCols(); j++) { // reset pointers for each node in row
      Node* bdownright = b->down->right;
      Node* bupright = b->up->right;
      b->up = bupright;
      b->down = bdownright;
      bdownright->up = b;
      bupright->down = b;

      if (r == 0) {
       headOfCol_[j] = b->left; // reset headOfCol_pointer for first row
      }

      b = b->right;
    }
  }
}

/**
 * Rotate column c (column 0 is the first) by count positions.
 * If column c is ABCDE (top to bottom) in grid g then column c
 * in grid g should be DEABC after the call g.rotateC(c, 2).
 * Rotate headOfRow_ if necessary.
 */
void Grid::rotateC(int c, int count) {
  c = c % numCols();

  if (count == 0 || count < 0) {
    return;
  }

  for (int i=0; i < count; i++) { // loops for each time shifted
    Node *b = headOfCol_[c];
    headOfCol_[c] = b->up;  // move headOfCol_[c] up

    for(int j=0; j < numRows(); j++) { // reset pointers for each node in column
      Node* brightdown = b->right->down;
      Node* bleftdown = b->left->down;
      b->right = brightdown;
      b->left = bleftdown;
      brightdown->left = b;
      bleftdown->right = b;

      if (c == 0) {
        headOfRow_[j] = b->up; // reset headOfRow_ for first column
      }

      b = b->down;
    }
  }
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Grid class. Clears headOfRow_ and headOfCol_ vectors.
 * Sets bwidth_, bheight_ to zero.
 * After clear() the grid represents an empty grid.
 */
void Grid::clear() {
  for (int i = 0; i<numCols(); i++) {
    Node* curr = headOfCol_[i];
    for (int j = 0; j<numRows(); j++) {
      Node* temp = curr;
      curr = curr->down;
      delete temp;
    }
  }

  headOfRow_.clear();
  headOfCol_.clear();
  bheight_ = 0;
  bwidth_ = 0;
}


/**
 * Makes the current Grid a copy of the "other" Grid.
 * The dimensions should be the same.  The Nodes should
 * contain the same blocks, but the Nodes should be newly
 * allocated. This function is used in both the copy
 * constructor and the assignment operator for Grids.
 */
void Grid::copy(Grid const& other) {
  clear(); // clear current Grid to avoid memory leak

  vector < vector <Node*>> A; // temporary 2D vector of ptrs to identical but
                              // new Nodes of other

  for( int j=0; j<other.numRows(); j++ ) {
    vector < Node * > temp;
    Node* curr = other.headOfRow_[j];
    for( int i=0; i<other.numCols(); i++ ) {
      Node* p = new Node(curr->block);
      temp.push_back(p);
      curr = curr->right;
    }
    A.push_back(temp);
  }
  // Set up the pointer structure between Nodes
  for( int j=0; j<other.numRows(); j++ ) {
    for( int i=0; i<other.numCols(); i++ ) {
      Node *p = A[j][i];
      // The following uses the C++ conditional operator
      // (also known as the C ternary operator):
      //
      // (condition) ? (if_true) : (if_false)
      //
      // which has the value if_true if the condition is true
      // and has the value if_false otherwise.
      p->up    = A[(j==0) ? other.numRows()-1 : j-1] [i];
      p->down  = A[(j==other.numRows()-1) ? 0 : j+1][i];
      p->left  = A[j] [(i==0) ? other.numCols()-1 : i-1];
      p->right = A[j] [(i==other.numCols()-1) ? 0 : i+1];
    }
  }
  for( int j=0; j<other.numRows(); j++ ) {
    headOfRow_.push_back(A[j][0]);
  }
  for( int i=0; i<other.numCols(); i++ ) {
    headOfCol_.push_back(A[0][i]);
  }

  bheight_ = other.bheight_;
  bwidth_ = other.bwidth_;
}
