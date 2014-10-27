// DirTree11.hpp

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
   #include <cstring>      // strerror()
   #include "sortedVector10.hpp"
   #include "ConditionalIncludes.hpp"
   
   using namespace std;
   
   namespace wcb
   {  class DirTree
      {  // This class maps out a directory tree.
         private:
         string                   _topDirName    ;  // Top level directory in tree
         vector<string>           _parentDirNames;  // Names of each director that contains files or subDirs
         vector<int   >           _parentDepths  ;  // # of subDirs under each parent directory.
         vector<vector  <string>> _childDirsV    ;  // Subdirectories immediately under parent.
         vector<vector  <string>> _childFilesV   ;  // Files immediately under parent.
         map<string, set<string>> _childDirsM    ;  // <dirName >, set<parentDirs> for DirNames  in multiple parentDirs
         map<string, set<string>> _childFilesM   ;  // <fileName>, set<parentDirs> for filenames in multiple parentDirs
         int _maxDepth                           ;  // Max depth to build tree
         int _nDepth                             ;  // Actual depty of tree from Top level.
   
         public:
         // Constructor
         DirTree(std::string topDirName1, int maxDepth1=0);
   
         int mapDirTree  (string topDirName1, int iDepth=0);   
         int printDirTree(FILE * outfp) const;
         int DirTree::printDirTree(FILE *outfp, int i) const;

         int findFilesByName    (vector<string>  filesOfInterest,
                                 vector<string>  &foundDirs     ,
                                 vector<string>  &foundFiles    ,
                                 vector<string>  &filesNotFound  ) const;

         int findFilesByName    (set   <string>  filesOfInterest,
                                 vector<string>  &foundDirsV    ,
                                 vector<string>  &foundFilesV   ,
                                 set   <string>  &filesNotFound ,
                                 set   <string>  &foundDirsSet  ,
                                 set   <string>  &foundFilesSet ,
                     map<string, set   <string>> &foundfilesMap  ) const;

         int findFilesEndingWith(vector<string>  ending         ,
                                 vector<string> &foundDirs      ,
                                 vector<string> &foundFiles     ,
                                 vector<string> &endingsNotFound ) const;
  
         // Gets
         string                  topDirName    () const;
         vector<string>          parentDirNames() const;
         vector<int   >          parentDepths  () const;
         vector<vector<string>>  childDirsV    () const;
         vector<vector<string>>  childFilesV   () const;
         map<string, set<string>>childDirsM    () const;
         map<string, set<string>>childFilesM   () const;
         int                     nDepth        () const;

      }; // End of class DirTree
   
   } // End of Namespace wcb

   #endif // DIRTREE

