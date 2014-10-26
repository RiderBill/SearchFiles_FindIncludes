// DirTree.hpp

#ifndef DIRTREE
   #define DIRTREE

   #include <stdlib.h>
   #include <stdio.h>
   #include <iostream>     // std::cout
   #include <fstream>      // std::ifstream, ofstream
   #include <string>
   #include <cstring>  // strerror()
   #include "sortedVector8.hpp"
   #include "ConditionalIncludes.hpp"
   
   using namespace std;
   
   namespace wcb
   {  class DirTree
      {  // This class maps out a directory tree.
         private:
         string                 _topDirName    ;
         vector<string>         _parentDirNames;
         vector<int   >         _parentDepths  ;
         vector<vector<string>> _childDirsV    ;
         vector<vector<string>> _childFilesV   ;
         int _maxDepth                         ;
         int _nDepth                           ;
   
         public:
         // Constructor
         DirTree(std::string topDirName1, int maxDepth1=0);
   
         int mapDirTree  (string &topDirName1, int iDepth=0);   
         int printDirTree(FILE * outfp);

         int DirTree::findFilesByName    (vector<string> filesOfInterest,
                                          vector<string> &foundDirs      ,
                                          vector<string> &foundFiles     ,
                                          vector<string> &filesNotFound   );

         int DirTree::findFilesEndingWith(vector<string>  ending         ,
                                          vector<string> &foundDirs      ,
                                          vector<string> &foundFiles     ,
                                          vector<string> &endingsNotFound );
   
         // Gets
         string                  topDirName     (void);
         vector<string>          parentDirNames (void);
         vector<int   >          parentDepths   (void);
         vector<vector<string>>  childDirsV     (void);
         vector<vector<string>>  childFilesV    (void);
         int                     nDepth         (void);
      }; // End of class DirTree
   
   } // End of Namespace wcb

   #endif // DIRTREE

