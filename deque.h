/**
 * @file queue.h
 * Definition of the Queue class.
 *
 * You **should not** modify this file for the PA!
 *
 */
#ifndef DEQUE_H
#define DEQUE_H

#include<vector>
using namespace std;

/**
 * Deque class: represents a standard deque, templated to hold elements of
 * any type. **You must only use the private member vector as your
 * storage space! You cannot create new private member variables to hold
 * your objects!**  We have given you two other integers that you might
 * find useful.
 *
 * You **should not** modify this file for the PA!
 *
 */
template <class T>
class Deque
{
  public:

    Deque();

    /**
     * Adds the parameter object to the "right" end of the Deque.
     *
     * @note This fuction should have O(1) behavior over n operations!
     * You may trust that the vector class resizes the array appropriately
     * for additions to the structure, so no resizing is necessary for 
     * function pushR.
     *
     * @param newItem object to be added to the Deque.
     */
    void pushR(T newItem);

    /**
     * Removes the object at the left of the Deque, and returns it to
     * the caller. You may assume that this function is only called
     * when the Deque is non-empty.
     *
     * To achieve amortized O(1) cost per operation, we will resize 
     * the array downward at strategic points of the computation. 
     * Specifically, you will maintain a member variable called L to 
     * denote the index of the leftmost data element in the deque.
     * If at any point the data will fit into array indices O..L-1,
     * the array should be reinitialized to hold the data beginning
     * at index 0 and occupying the next num cells (where num is the
     * member variable storing the number of data elements in the 
     * structure). 
     *
     * @note This function should have O(1) behavior over n operations!
     *
     * @return The item that used to be at the left of the Deque.
     */
    T popL();

    /**
     * Removes the object at the right of the Deque, and returns it to
     * the caller. You may assume that this function is only called
     * when the Deque is non-empty.
     *
     * @note This function should have O(1) behavior over n operations!
     * To achieve amortized O(1) cost per operation, we will resize 
     * the array downward at strategic points of the computation. 
     * Specifically, you will maintain a member variable called L to 
     * denote the index of the leftmost data element in the deque.
     * If at any point the data will fit into array indices O..L-1,
     * the array should be reinitialized to hold the data beginning
     * at index 0 and occupying the next num cells (where num is the
     * member variable storing the number of data elements in the 
     * structure). 
     *
     * @return The item that used to be at the right of the Deque.
     */
    T popR();

    /**
     * Finds the object at the left of the Deque, and returns it to
     * the caller. Unlike popL(), this operation does not alter the
     * deque. You may assume that this function is only called when the
     * Deque is non-empty.
     *
     * @note This function should have O(1) behavior.
     *
     * @return The item at the left of the deque.
     */
    T peekL();

    /**
     * Finds the object at the right of the Deque, and returns it to
     * the caller. Unlike popR(), this operation does not alter the
     * deque. You may assume that this function is only called when the
     * Deque is non-empty.
     *
     * @note This function should have O(1).
     *
     * @return The item at the right of the deque.
     */
    T peekR();

    /**
     * Determines if the Deque is empty.
     *
     * @note This function should have O(1).
     *
     * @return bool which is true if the Deque is empty, false
     *	otherwise.
     */
    bool isEmpty() const;

  private:
    vector<T> data;  /* Store the deque data here! */
    int n1;  
    int n2; 
};

#include "deque.cpp"
#endif
