/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/* No argument constructor */
template <class T>
Deque<T>::Deque():n1(-1),n2(0){}
// n2 = index of first element
// n1 = index of last element


/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
  data.push_back(newItem);
  n1++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
	T left = data[n2];
  vector<T> temp;
  n2++;

  if (n1-n2 < n2 - 1) {
    int n2b = n2;
    for(int i = 0; i<(n1 - n2 + 1); i++) {
      temp.push_back(data[n2b]);
      n2b++;
    }

    data.clear();
    data = temp;
    n1 = data.size() - 1;
    n2 = 0;
  }
	return left;
}



/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T right = data[n1];
    data.pop_back();
    n1--;

    if(n1-n2 <= n2-1) {
      vector<T> temp;
      int n2b = n2;
      for(int i = 0; i < n1-n2+1; i++) {
        temp.push_back(data[n2b]);
        n2b++;
      }

      data.clear();
      data=temp;
      n1=data.size()-1;
      n2=0;
    }
    return right;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    T temp = data[n2];
    return temp;
}



/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    T temp = data[n1];
    return temp;
}



/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n1<0 || n2>n1;
}
