// DirTree9.hpp

#ifndef DIRTREE
   #define DIRTREE

   #include <cstdlib>
   #include <cstdio>
   #include <iostream>     // std::cout
   #include <fstream>      // std::ifstream, ofstream
   #include <string>
   #include <vector>
   #include <set>
   #include <map>
   #include <cstring>  // strerror()
   #include "sortedVector10.hpp"
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
         map<string, vector<string>> 
         int _maxDepth                         ;
         int _nDepth                           ;
   
         public:
         // Constructor
         DirTree(std::string topDirName1, int maxDepth1=0);
   
         int mapDirTree  (string topDirName1, int iDepth=0);   
         int printDirTree(FILE * outfp) const;

         int DirTree::findFilesByName    (vector<string>  filesOfInterest,
                                          vector<string>  &foundDirs     ,
                                          vector<string>  &foundFiles    ,
                                          vector<string>  &filesNotFound  ) const;

         int DirTree::findFilesByName    (set   <string>  filesOfInterest,
                                          vector<string>  &foundDirsV    ,
                                          vector<string>  &foundFilesV   ,
                                          set   <string>  &filesNotFound ,
                                          set   <string>  &foundFilesSet ,
                              map<string, set   <string>> &foundfilesMap  ) const;

         int DirTree::findFilesEndingWith(vector<string>  ending         ,
                                          vector<string> &foundDirs      ,
                                          vector<string> &foundFiles     ,
                                          vector<string> &endingsNotFound ) const;
   
         // Gets
         string                  topDirName    () const;
         vector<string>          parentDirNames() const;
         vector<int   >          parentDepths  () const;
         vector<vector<string>>  childDirsV    () const;
         vector<vector<string>>  childFilesV   () const;
         int                     nDepth        () const;

      }; // End of class DirTree
   
   } // End of Namespace wcb

   #endif // DIRTREE

