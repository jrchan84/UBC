/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg);
}

QTree::~QTree(){
  clear();
}

QTree::QTree(const QTree & other) {
  copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)     // drawframe can be checked in write
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
  im = imIn;
  int width = biggestPow2(imIn.width());   // part of img will be cut off if not square?
  int height = biggestPow2(imIn.height());
  pair<int,int> ul(0,0); // root has origin as upperleft coords

  if (width < height) {
    root = new Node(imIn, ul, width, NULL); // root's parent is NULL
  } else {
    root = new Node(imIn, ul, height, NULL);
  }

  numLeaf = 1;
  PriorityQ.push(root);

  while (!PriorityQ.empty() && numLeaf < leafBound) { // leafBound is approximate. can overshoot one split?
    Node* n = PriorityQ.top();
    PriorityQ.pop();
    //numLeaf--;
    if (isLeaf(n)){
      split(n);
    }
    //numLeaf += 4;
  }
}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
  im = imIn;
  int width = biggestPow2(imIn.width());   // part of img will be cut off if not square?
  int height = biggestPow2(imIn.height());
  pair<int,int> ul(0,0); // root has origin as upperleft coords

  if (width < height) {
    root = new Node(imIn, ul, width, NULL); // root's parent is NULL
  } else {
    root = new Node(imIn, ul, height, NULL);
  }

  PriorityQ.push(root);
  numLeaf = 1;

  while (!PriorityQ.empty() && numLeaf < leafBound) { // leafBound is approximate. can overshoot one split?
    Node* n = PriorityQ.top();
    PriorityQ.pop();
    //numLeaf--;
    if (isLeaf(n)) {
      split(n);
    }
    //numLeaf += 4;
  }
}


bool QTree::isLeaf( Node *t ) {
  //leaf = 4 NULL pointers and can't be NULL itself;
  return (t != NULL && t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL);
}

void QTree::split( Node *t ) {
  int newSize = t->size / 2;

  //creating 4 leaf nodes
  pair<int,int> nwTopLeftCorner(t->upLeft.first,t->upLeft.second);
  pair<int,int> neTopLeftCorner(t->upLeft.first + newSize, t->upLeft.second);
  pair<int,int> swTopLeftCorner(t->upLeft.first,t->upLeft.second + newSize);
  pair<int,int> seTopLeftCorner(t->upLeft.first + newSize, t->upLeft.second + newSize);

  Node* nwLeaf = new Node(im, nwTopLeftCorner, newSize, t);
  Node* neLeaf = new Node(im, neTopLeftCorner, newSize, t);
  Node* swLeaf = new Node(im, swTopLeftCorner, newSize, t);
  Node* seLeaf = new Node(im, seTopLeftCorner, newSize, t);

  //setting t pointers
  t->nw = nwLeaf;
  t->ne = neLeaf;
  t->sw = swLeaf;
  t->se = seLeaf;

  //update private variables
  numLeaf += 3;
  t->avg.a = 0; // make parent node that now consists of four leafs transparent.

  //add to structure - order doesnt matter since priority?
  PriorityQ.push(nwLeaf);
  PriorityQ.push(neLeaf);
  PriorityQ.push(swLeaf);
  PriorityQ.push(seLeaf);


  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.
  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.

  //four cases if parent != NULL; eg. nw -> split NNbr, WNbr

  // NOTE ***************
  // Ada, geo, and klee balanced images are slightly different from solution.
  // I've tried changing the order of things with no result. Everything else is identical.

  if (balanced) {
    if (t->parent != NULL) {

      int cases = (t == t->parent->nw) ? 0    // got sick of long if/else statements
                : (t == t->parent->ne) ? 1    // hope the switch statements are more readable
                : (t == t->parent->sw) ? 2    // switch can only use int? :(
                : (t == t->parent->se) ? 3
                :                        4;

      switch (cases) {
        case 0:
            if (isLeaf(NNbr(t->parent))) { // check if already split
              split(NNbr(t->parent));      // if is a leaf, we split
            }
            if (isLeaf(WNbr(t->parent))) {
              split(WNbr(t->parent));
            }
            break;
        case 1:
            if (isLeaf(NNbr(t->parent))) {
              split(NNbr(t->parent));
            }
            if (isLeaf(ENbr(t->parent))) {
              split(ENbr(t->parent));
            }
            break;
        case 2:
            if (isLeaf(SNbr(t->parent))) {
              split(SNbr(t->parent));
            }
            if (isLeaf(WNbr(t->parent))) {
              split(WNbr(t->parent));
            }
            break;
        case 3:
            if (isLeaf(SNbr(t->parent))) {
              split(SNbr(t->parent));
            }
            if (isLeaf(ENbr(t->parent))) {
              split(ENbr(t->parent));
            }
            break;
        default:
          return;
      }
    }
  }
}

/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {
  //go up tree to parent, then to respective neighbors, recursing if necessesary in north cases
  if (t->parent == NULL) {
    return NULL;
  }

  int cases = (t == t->parent->nw) ? 0
            : (t == t->parent->ne) ? 1
            : (t == t->parent->sw) ? 2
            : (t == t->parent->se) ? 3
            :                        4;

  Node* parentNorthNbr = NNbr (t->parent); // initialization cant occur in switch cases

  switch (cases) {
    case 0:
        if (parentNorthNbr == NULL) {      // nw case -> parents' north neighbors' sw leaf
          return NULL;
        } else {
          return parentNorthNbr->sw;
        }
        break;
    case 1:
        if (parentNorthNbr == NULL) {      // ne case -> parents' north neighbors' se leaf
          return NULL;
        } else {
          return parentNorthNbr->se;
        }
        break;
    case 2:
        return t->parent->nw;               // sw case -> parents nw leaf
        break;
    case 3:
        return t->parent->ne;               //se case -> parents ne leaf;
        break;
    default:
        return NULL;
  }
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {
  if (t->parent == NULL) {
    return NULL;
  }

  int cases = (t == t->parent->nw) ? 0
            : (t == t->parent->ne) ? 1
            : (t == t->parent->sw) ? 2
            : (t == t->parent->se) ? 3
            :                        4;

  Node* parentSouthNbr = SNbr (t->parent);

  switch (cases) {
    case 0:
        return t->parent->sw;
        break;
    case 1:
        return t->parent->se;
        break;
    case 2:
        if (parentSouthNbr == NULL) {
          return NULL;
        } else {
          return parentSouthNbr->nw;
        }
        break;
    case 3:
        if (parentSouthNbr == NULL) {
          return NULL;
        } else {
          return parentSouthNbr->ne;
        }
        break;
    default:
        return NULL;
  }
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {
  if (t->parent == NULL) {
    return NULL;
  }

  int cases = (t == t->parent->nw) ? 0
            : (t == t->parent->ne) ? 1
            : (t == t->parent->sw) ? 2
            : (t == t->parent->se) ? 3
            :                        4;

  Node* parentEastNbr = ENbr (t->parent);

  switch (cases) {
    case 0:
        return t->parent->ne;
        break;
    case 1:
        if (parentEastNbr == NULL) {
          return NULL;
        } else {
          return parentEastNbr->nw;
        }
        break;
    case 2:
        return t->parent->se;
        break;
    case 3:
        if (parentEastNbr == NULL) {
          return NULL;
        } else {
          return parentEastNbr->sw;
        }
        break;
    default:
        return NULL;
  }
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {
  if (t->parent == NULL) {
    return NULL;
  }

  int cases = (t == t->parent->nw) ? 0
            : (t == t->parent->ne) ? 1
            : (t == t->parent->sw) ? 2
            : (t == t->parent->se) ? 3
            :                        4;

  Node* parentWestNbr = WNbr (t->parent);

  switch (cases) {
    case 0:
        if (parentWestNbr == NULL) {
          return NULL;
        } else {
          return parentWestNbr->ne;
        }
        break;
    case 1:
        return t->parent->nw;
        break;
    case 2:
        if (parentWestNbr == NULL) {
          return NULL;
        } else {
          return parentWestNbr->se;
        }
        break;
    case 3:
        return t->parent->sw;
        break;
    default:
        return NULL;
  }
}


bool QTree::write(string const & fileName){
  // draw each leaf in the priority_queue
  // b/c of leafBound, priority_queue still has the nodes needed - havent been popped yet.
  while (!PriorityQ.empty()) {
    Node* top = PriorityQ.top();
    PriorityQ.pop();
    if (isLeaf(top)) {
    int size = top->size - 1; // - 1 from bounds checking
    for (int i = top->upLeft.first; i <= top->upLeft.first + size; i++) { // loop over every pixel
      for (int j = top->upLeft.second; j <= top->upLeft.second + size; j++) {
        RGBAPixel* p = im.getPixel(i,j);
        if (drawFrame) {
          // when i = top and bottom lines, j = left and right lines of square, drawFrame
          if (i == top->upLeft.first || i == top->upLeft.first + size || j == top->upLeft.second || j == top->upLeft.second + size) {
            *p = frameColor;  // color with frameColor (RGBAPixel)
          } else {
            *p = top->avg;
          }
        } else { // drawFrame == false, color with leafs' average color (RGBAPixel)
          *p = top->avg;
        }
      }
    }
    }
  }

  // include the following line to write the image to file.
  return(im.writeToFile(fileName));
}

void QTree::clear() {
  clear(root);
}

void QTree::clear(Node* root) {
  //recurse down to leafs, then delete w/ control flow
  if (root == NULL) {
    return;
  }

  clear(root->nw);
  clear(root->ne);
  clear(root->sw);
  clear(root->se);

  delete root;
  root = NULL;
}


void QTree::copy(const QTree & orig) {
  // does not free memory! called by = and copy constr.
  Node* origRoot = orig.root;

  numLeaf = orig.numLeaf;
  im = orig.im;
  leafBound = orig.leafBound;
  balanced = orig.balanced;
  drawFrame = orig.drawFrame;
  frameColor = orig.frameColor;
  PriorityQ = orig.PriorityQ;

  root = copy(origRoot, NULL); // NULL as root's parent is NULL
}

// NOTE***
// running ./pa3 with valgrind shows no mem leak, with all heap blocks freed.
//         ./pa3 also runs without errors or warnings. However, Memory Test fails in autograder
//         after implementing copy.


QTree::Node* QTree::copy(Node* otherTreeNode, Node* parent) {
  // need to build down from root. In clear we delete leafs so recursion works. Here...
  // need to pass parent field otherwise the nodes in this tree point to old ones.
  Node* newNode;

  if (otherTreeNode != NULL) {
    newNode = new Node(im, otherTreeNode->upLeft, otherTreeNode->size, parent);
    newNode->nw = copy(otherTreeNode->nw, newNode);
    newNode->ne = copy(otherTreeNode->ne, newNode);
    newNode->sw = copy(otherTreeNode->sw, newNode);
    newNode->se = copy(otherTreeNode->se, newNode);
  } else {
    return NULL;
  }

  return newNode;
}
