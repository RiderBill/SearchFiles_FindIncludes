    // sortedVector10.hpp
    // The following is based on Matt Austern's article,
    // "Why you shouldn't use set (and what you should use instead)"
    // (http://lafstern.org/matt/col1.pdf).
    // I modified it slightly, mostly just reformatting and adding comments.
    // Also changed sorted_vector to sortedVector (cammelCase)
    
    #include <vector>
    #include <algorithm>  // std::lower_bound

    #ifndef SORTEDVECTOR
    #define SORTEDVECTOR

namespace wcb
{
   // template<typename T> (typename std::vector)<T>::iterator  // Compiles OK, haven't tested the resulting code
   // template<typename T> (typename std::vector<T>)::iterator  // Compiles OK, haven't tested the resulting code
   //   template<typename T> (typename std::vector<T>::iterator)  // OK, code tested and works as expected.
    template<typename T>  typename std::vector<T>::iterator   // OK, now this works.  It didn't work yesterday.
                  insertIntoVector(std::vector<T>& vec, T & val);
      

// The rest creates a new container class, sortedVector.  It needs more work
// to be fully vast.


} // namespace wcb

#endif //SORTEDVECTOR