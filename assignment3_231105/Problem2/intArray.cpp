/* P2:
 * File: intArray.cpp
 * ------------------
 * This file inplements the intarray.h interface.
 */
#include <iostream>
#include <cstring>
// #include "CSC3002OJActive/assignment3/intArray.h"  // For OJ test
#include "intArray.h"  // For local test

using namespace std;
/*
 * Implementation notes: IntArray constructor and destructor
 * ---------------------------------------------------------
 * The constructor allocates an array to hold the values and initializes
 * each element to 0; the constructor must also store the effective size
 * in the variable nElements.  The destructor must take responsibility
 * for freeing the dynamic array.
 */

IntArray::IntArray(int n) {
   array = new int[n];
   memset(array, 0, n*sizeof(int));
   nElements = n;
}

IntArray::~IntArray() {
   delete[] array;
   array = nullptr;
   nElements = 0;
}

/*
 * Implementation notes: size, get, and set
 * ----------------------------------------
 * Given the data structure, each of these methods is straightforward.
 */

int IntArray::size() {
   return nElements;
}

int IntArray::get(int index) {
   if (index < 0 || index >= nElements) {
      error("Index range out of bounds!");
      return 0;
   }
   return array[index];
}

void IntArray::put(int index, int value) {
   if (index < 0 || index >= nElements) {
      error("Index range out of bounds!");
      return;
   }
   array[index] = value;
}

/*
 * Implementation notes: operator []
 * ---------------------------------
 * The selction operator is identical to get except for the fact that it
 * returns its result by reference.
 */

int & IntArray::operator[](int index) {
   if (index < 0 || index >= nElements) {
      error("Index range out of bounds!");
      return *new int(0);
   }
   return array[index];
}

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * These methods make it possible to pass a IntArray by value or
 * assign one IntArray to another.
 */

IntArray::IntArray(const IntArray & src) {
   array = nullptr;
   nElements = 0;
   deepCopy(src);
}

IntArray & IntArray::operator=(const IntArray & src) {
   deepCopy(src);
   return *this;
}

/*
 * Implementation notes: deepCopy
 * ------------------------------
 * Copies the data from the src parameter into the current object.
 * All dynamic memory is reallocated to create a "deep copy" in
 * which the current object and the source object are independent.
 */

void IntArray::deepCopy(const IntArray & src) {
   nElements = src.nElements;
   int *newArray = new int[src.nElements];
   for (int i = 0; i < src.nElements; i++) {
      newArray[i] = src.array[i];
   }
   delete[] array;
   array = newArray;
}

/* DO NOT modify this part*/
int main() {
   int id;
   int size_num;
   cin>>id;
   cin>>size_num;

   if(id==1){
      IntArray array(size_num);
      cout << "array.size(): " << integerToString(array.size()) << '\n' << endl;
      for (int i = 0; i < size_num; i++) {
         cout << "array.get(): " << integerToString(array.get(i)) << endl;
         cout << "array.put(" << i << ", " << i << ");" << endl;
         array.put(i, i);
         cout << "array.get(): " << integerToString(array.get(i))<< '\n' << endl;
      }
      array.get(-1);
      array.put(size_num+1, size_num+1);
   }
   if(id == 2){
      IntArray array(size_num);
      cout << "array.size(): " << integerToString(array.size()) << '\n' << endl;
      for (int i = 0; i < size_num; i++) {
         cout << "array.get(): " << integerToString(array.get(i)) << endl;
         cout << "array[" << i << "] = " << i << ";" << endl;
         array[i] = i;
         cout << "array.get(): " << integerToString(array.get(i))<< '\n' << endl;
      }
   }
   if(id == 3){
      IntArray v1(size_num);
      cout << "v1.size(): " << integerToString(v1.size()) << '\n' << endl;
      for (int i = 0; i < size_num; i++) {
         cout << "v1.get(): " << integerToString(v1.get(i)) << endl;
         cout << "v1[" << i << "] = " << i << ";" << endl;
         v1[i] = i;
         cout << "v1.get(): " << integerToString(v1.get(i))<< '\n' << endl;
      }
      IntArray v2;
      v2 = v1;
      cout << "v2.size(): " << integerToString(v2.size()) << '\n' << endl;
      for (int i = 0; i < v2.size(); i++) {
         cout << "v2.get(): " << integerToString(v2.get(i)) << endl;
      }
      IntArray v3;
      v3 = IntArray(v1);
      cout << '\n' << "v3.size(): " << integerToString(v3.size()) << '\n' << endl;
      for (int i = 0; i < v3.size(); i++) {
         cout << "v3.get(): " << integerToString(v3.get(i)) << endl;
      }
   }
   return 0;
}

