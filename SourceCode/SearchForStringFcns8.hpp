#include <stdlib.h>
 #include <stdio.h>
  #include <TCHAR.h>
   #include <iostream>     // std::cout
    #include <fstream>      // std::ifstream, ofstream
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

