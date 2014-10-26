    // sortedVector10.cpp
    // The following is based on Matt Austern's article,
    // "Why you shouldn't use set (and what you should use instead)"
    // (http://lafstern.org/matt/col1.pdf).
    // I modified it slightly, mostly just reformatting and adding comments,
    // but I also changed it to return an iterator to the inserted element.
    // Also changed sorted_vector to sortedVector (cammelCase)
    
    #include <vector>
    #include <algorithm>  // std::lower_bound

namespace wcb
{
   // template<typename T> (typename std::vector)<T>::iterator  // Compiles OK, haven't tested the resulting code
   // template<typename T> (typename std::vector<T>)::iterator  // Compiles OK, haven't tested the resulting code
   // template<typename T> (typename std::vector<T>::iterator)  // OK, code tested and works as expected.
    template<typename T>  typename std::vector<T>::iterator   // OK, now this works.  It didn't work yesterday.
                  insertIntoVector(std::vector<T>& vec, T & val)
    {  // Insert t into vector vec such that v remains in sorted order
       // and all elements of vec are unique (like a set).
       // This only makes sense if the vector elements are
       // maintained in sorted order.
       // A sorted vector might perform better than a set if
       // there are many more access operations than insertions
       // (smaller storage, fast access via [], ... at the cost
       // of much slower insertions).
       // Note: Type T must have a defined < operator.
       /// Todo: overload this function to allow passing a Compare()
       /// function pointer.
    
          // std::lower_bound() gives log2(N)-O(1) performance.
       vector<T>::iterator itr = lower_bound(vec.begin(), vec.end(), val);
       if ( (vec.end() == itr) || (val < *itr) )
       {  itr = vec.insert(itr, val);
       }
       return itr;  // return iterator to val in vec.
    } // End of insertIntoVector();

// The rest creates a new container class, sortedVector.  It needs more work
// to be fully vast.

//template<typename T> class sortedVector : std::vector<T>
//{  // Constructors
//   sortedVector(const allocator_type& alloc = allocator_type())
//              : vector(alloc)
//   { // Empty container
//   }
//   sortedVector(size_type nelements, const value_type& valT= value_type(),
//                 const allocator_type& alloc = allocator_type())
//              : vector(nelements, valT, alloc )
//   { // nelements elements.  Must be sorted.
//   // do stuff here
//   }
//// template <class InputIterator>
//// sortedVector(InputIterator firstItr, InputIterator lastItr,
////              const allocator_type& alloc = allocator_type())
////            : vector(firstItr, lastItr, alloc)
//// {}
//   sortedVector(const vector& vect)
//              : vector(vect)
//   {}
//}; // end of class sortedVector
//                
//
//template <class T, class Compare = std::less<T> >  // less<T> must exist for type T
//struct sortedVector1 : std::vec
//{  std::vector<T> V;
//   Compare cmp;
//   typedef typename std::vector<T>::      iterator       iterator;
//   typedef typename std::vector<T>::const_iterator const_iterator;
//   std::iterator       begin()       { return V.begin(); }
//   std::iterator       end  ()       { return V.end  (); }
//   std::const_iterator begin() const { return V.begin(); }
//   std::const_iterator end  () const { return V.end  (); }
// //... I'm not sure what is missing here. Austern says he omitted the
// //   "boilerplate" that's needed to support the complete STL container
// //   interface.
// //
// //   Here is part of the missing stuff:
//
//   sortedVector1(const Compare& c = std::Compare()) : V(), cmp(c) {}
//   template <class InputIterator> sortedVector1(InputIterator first,
//                                               InputIterator last,
//                                         const Compare& c = Compare())
//                                 : V(first, last), cmp(c)
//   {  std::sort(begin(), end(), cmp);
//   }
// //... more "boilerplate" needed to support the complete STL
// //    container interface I guess.
//
//   iterator insert(const T& t)
//   {  iterator i = lower_bound(begin(), end(), t, cmp);
//      if (i == end() || cmp(t, *i)) V.insert(i, t);
//      return i;
//   }
//
//   const_iterator find(const T& t) const
//   {  const_iterator i = lower_bound(begin(), end(), t, cmp);
//     return i == end() || cmp(t, *i) ? end() : i;
//   }
//
//     // Ok, here is something Austern omitted:
//   size_t size() { return V.size(); }
//}; // struct sortedVector1

} // namespace wcb
