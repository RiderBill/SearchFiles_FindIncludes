// I could have done this with grep, but I was working in a Windows platform at the time.
#include <stdlib.h> 
 #include <stdio.h>    // There must have been a good reason why I indented the
  #include <TCHAR.h>   // include statements. I may have been using the project
    #include <fstream> // directory to test the code, e.g., test for leading space before "#include"
   #include <iostream> // std::cout std::ifstream, ofstream
     #include <string>
      #include <cstring>      // strerror()
       #include <vector>

using namespace std;

int findStringInFiles(vector<string>  searchString, 
                       vector<string>  parents     ,
                       vector<string>  files       ,
                       vector<string> &parentsOut  ,
                       vector<string> &filesOut    ,
                       vector<string> &linesOut     );

bool searchForString  (ifstream     &ifs      , string const   &searchText, size_t &position);
bool searchForString  (FILE         *fp       , string const   &searchText, size_t &position);
bool searchForString  (string const &targText , string const   &searchText, size_t &position);
                                                               
bool searchForNStrings(ifstream     &ifs      , string const   &searchText    , 
                       size_t        maxToFind,
                       size_t       &nFound   , vector<size_t> &foundPositions );

bool searchForNStrings(FILE         *fp       , string const   &searchText    ,
                       size_t        maxToFind,
                       size_t       &nFound   , vector<size_t> &foundPositions );

bool searchForNStrings(string const &targText , string const   &searchText    ,
                       size_t        maxToFind, size_t          startPos      , 
                       size_t       &nFound   , vector<size_t> &foundPositions );

