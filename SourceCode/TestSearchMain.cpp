#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>      // strerror()
#include <vector>
// Oh, hell.  The stackoverflow question referred to Java.  Well, I needed to test this anyway.
using namespace std;
bool searchForString  (string const &targText , string const   &searchText, size_t &position);
bool searchForNStrings(string const &targText , string const   &searchText    ,
                       size_t        maxToFind, size_t          startPos      , 
                       size_t       &nFound   , vector<size_t> &foundPositions );
int main(int argc, char const *argv[])
{  size_t nFound;
   vector<size_t> foundPos;
   string str1("Nagash is cool really cool");
   string str2("Nagash is coolish"         );

   searchForNStrings(str1, string("cool"), 0, 0, nFound, foundPos);
   searchForNStrings(str2, string("is"  ), 0, 0, nFound, foundPos);

   return 0;
} // End of main()

