#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include "ConditionalIncludes.hpp"
#include "DirTree.hpp"

using namespace std;

namespace wcb
{  class DirTree
   {  // This class maps out a directory tree.

      private:
      string                 _topDirName    ;
      vector<string>         _parentDirNames;
      vector<vector<string>> _subDirsV      ;
      vector<vector<string>> _filesV        ;
      int _maxDepth                         ;
      int _nDepth                           ;

      public:
      // Constructor
      DirTree(std::string &topDirName1, int maxDepth1=0);

      int mapDirTree  (string topDirName1);
   
      int printDirTree();
      
      string                  topDirName    ;
      vector<string>          parentDirNames;
      vector<vector<string>>  subDirsV      ;
      vector<vector<string>>  filesV        ;
      int                     nDepth        ;

   }; // End of class DirTree

} // End of Namespace wcb

