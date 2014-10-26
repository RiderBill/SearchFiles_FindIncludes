#include <cstdlib>
#include <cstdio>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <algorithm>  // transsform() , e.g. toupper
#include "./sortedVector8.hpp"
#include "ConditionalIncludes.hpp"
#include "DirTree9.hpp"

using namespace std;

namespace wcb
{  // class DirTree
   // Constructor
   DirTree::DirTree(std::string topDirName1, int maxDepth1) :
      _topDirName(topDirName1),
      _maxDepth  (maxDepth1  ),
      _nDepth    (0          )
   {  int nDepth = 0;
      _parentDirNames.clear();
      _childDirsV    .clear();
      _childFilesV  . clear();
      _parentDepths  .clear();

      mapDirTree(_topDirName, nDepth);
   } // End of DirTree(std::string &topDirName1, int maxDepth1=0) constructor


   int DirTree::mapDirTree  (string parentName, int iDepth)
   {  int status;
      struct dirent *drnt;
      DIR *dr;
      vector<string> subDirs; // Subdirectories just in this parent directory.
      vector<string> files;   // Files just in this parent directory.
   
      if(_nDepth == 0)
      {  _parentDirNames.clear();
         _childDirsV    .clear();
         _childFilesV   .clear();
      }

      if(   (_maxDepth != 0      )
         && (_nDepth >= _maxDepth) )
      {  status = 0;
         return status;
      }
         // Add trailing slash if not already present
      if( parentName[parentName.size()-1] != '\\')
      {   parentName += "\\";
      }

      dr=opendir(parentName.c_str());
      if (dr!=NULL)
      {  status = 1;
         iDepth ++;
         if (iDepth > _nDepth) _nDepth = iDepth;
         _parentDirNames.push_back(parentName);
         _parentDepths  .push_back(iDepth    );
         while ((drnt=readdir(dr)) != NULL)
         {  // Could be directory or file.
            string fdName = drnt->d_name;
            if (  (strncmp( drnt->d_name, "." , 1 ) == 0 )
                ||(strncmp( drnt->d_name, "..", 2 ) == 0 ) ) continue;
            fdName = parentName + fdName;
   
            if ( drnt->d_type == DT_DIR)
            {  // Directory
               string subDirName = fdName + string("\\");
               insert_into_vector(subDirs, subDirName);  // Alphabetical order
            }
            else
            {  // File
               string fileName   = fdName;
               insert_into_vector(files  , fileName  );  // Alphabetical order
            }
         } // End of while(drnt!=NULL)
         closedir(dr);
   
         _childDirsV .push_back(subDirs); // These need to stay in one-for-one
         _childFilesV.push_back(files  ); // correspondence, and in the order found(?)
                                          // Anyway,they are vectors of vectors, so
                                          // I'm not sure how they would be sorted.
                                          // I think it is going to work out well anyway.
   
         for (vector<string>::iterator it  = subDirs.begin() ;
                                       it != subDirs.end  (); ++it)
         {  mapDirTree(*it, iDepth);
         }
      } // End of if(dr!=NULL)
      else
      {  fprintf_s(stderr, "Unable to open %s as a directory\n", parentName.c_str());
      // closedir(dr); // Not sure if this is necessary.
         status = 0;
      }  // End of else if(dr!=NULL)

      return status;
   }; // End of getDirTree()
   
   
   int DirTree::printDirTree(FILE *outfp)
   {  int nParentDirNames = _parentDirNames.size();
   
      for(int iParent=0; iParent<nParentDirNames; ++iParent)
      {  fprintf_s(outfp, "Contents of %s:\n", _parentDirNames[iParent].c_str());
         for (vector<string>::iterator it  = _childDirsV[iParent].begin();
                                            it != _childDirsV[iParent].end()  ;
                                          ++it)
         {  fprintf_s(outfp, "            %s\n", it->c_str());
         }
         for (vector<string>::iterator it  = _childFilesV[iParent].begin();
                                            it != _childFilesV[iParent].end()  ;
                                          ++it)
         {  fprintf_s(outfp, "            %s\n", it->c_str());
         }
      }

      return 0;
   } // End of printDirTree()

   
   int DirTree::findFilesByName(vector<string> filesOfInterest,
                                vector<string> &foundDirs      ,
                                vector<string> &foundFiles     ,
                                vector<string> &filesNotFound   )
   {    // Find the referenced files contained in the directory tree
      int nFoundTotal = 0;
      int nfoi        = filesOfInterest.size();
      int nParents    = _parentDirNames.size();

        // Loop on files of interest.
      for (int ifoi=0; ifoi<nfoi; ifoi++)
      {  int nfound = 0;
   
         string foi(filesOfInterest[ifoi]);
         
           // Convert to all uppercase for comparison.
         string foiUpper(foi);
         transform(foi.begin(), foi.end(), foiUpper.begin(), ::toupper);
         
           // Loop through parent directories
         for(int iParent=0; iParent<nParents; iParent++)
         {    // Loop through child Files
            int nChildFiles = _childFilesV[iParent].size();
            for(int iChild=0; iChild<nChildFiles; iChild++)
            {    //First get just the file name w/o path.
               int lastSlashPos =  _childFilesV[iParent][iChild]
                                  .find_last_of("/\\");
               string fileName  = _childFilesV[iParent][iChild]
                                  .substr(lastSlashPos+1);

               if(foi.size() != fileName.size())
               {  continue;
               }

                 // Convert to all uppercase for comparison.
               string ffileUpper(fileName);
               transform( ffileUpper.begin(),
                          ffileUpper.end  (),
                          ffileUpper.begin(), ::toupper);
               if(foiUpper.compare(ffileUpper) == 0)
               {  nfound++;
                  nFoundTotal++;
                    // Can't use the insert_into_vector function here.
                    // might find the file more than once, and more than one
                    // file might be in the same subdirectory.
                  foundFiles.push_back(fileName                );
                  foundDirs .push_back(_parentDirNames[iParent]);
               }
               
            } // End of for(iChild)
         } // End of for(iParent)
         if (nfound == 0)
         {  // Keep these in order and non-redundant.
            insert_into_vector(filesNotFound, filesOfInterest[ifoi]);
         }
      } // End of for(ifoi)

      return nFoundTotal;
   } // End of findFilesByName()
   
   
   int DirTree::findFilesEndingWith(vector<string>  endings        ,
                                    vector<string> &foundDirs      ,
                                    vector<string> &foundFiles     ,
                                    vector<string> &endingsNotFound )
   {  // Find the files with specified endings contained in the directory tree
      int nFoundTotal = 0;
      int nEnds    = endings        .size();
      int nParents = _parentDirNames.size();
      for (int iEnd=0; iEnd<nEnds; iEnd++)
      {  int nfound = 0;

         string ending(endings[iEnd]);
         
         // Convert to all uppercase for comparison.
         string endUpper(ending);
         transform(ending.begin(), ending.end(), endUpper.begin(), ::toupper);
         
         // Loop through parent directories
         for(int iParent=0; iParent<nParents; iParent++)
         {  // Loop through child Files
            int nChildFiles = _childFilesV[iParent].size();
            for(int iChild=0; iChild<nChildFiles; iChild++)
            {  //First get just the file name w/o path.
               int lastSlashPos  = _childFilesV[iParent][iChild]
                                  .find_last_of("/\\");
               string fileName   = _childFilesV[iParent][iChild]
                                   .substr(lastSlashPos+1);

               size_t endLen = ending  .size();
               size_t fnLen  = fileName.size();
               if(endLen > fnLen)
               {  continue;
               }

               // Convert end chars of file to all uppercase for comparison.
               string fnTail = fileName.substr(fnLen - endLen);
               transform( fnTail.begin(), fnTail.end(),
                          fnTail.begin(), ::toupper    );
               if(endUpper.compare(fnTail) == 0)
               {  nfound++;
                  nFoundTotal++;
                    // Can't use the insert_into_vector function here.
                    // might find the file more than once, and more than one
                    // file might be in the same subdirectory.
                  foundFiles.push_back(fileName                );
                  foundDirs .push_back(_parentDirNames[iParent]);
               }
               
            } // End of for(iChild)
         } // End of for(iParent)
         if (nfound == 0)
         {  // Keep these in order and non-redundant.
             insert_into_vector(endingsNotFound, endings[iEnd]);
         }
      } // End of for(iEnd)

      return nFoundTotal;
  } // End of findFilesEndingWith()

   string DirTree::topDirName                  () {  return _topDirName    ; }
   vector<string> DirTree::parentDirNames      () {  return _parentDirNames; }
   vector<int   > DirTree::parentDepths        () {  return _parentDepths  ; }
   vector<vector<string>>  DirTree::childDirsV () {  return _childDirsV    ; }
   vector<vector<string>>  DirTree::childFilesV() {  return _childFilesV   ; }
   int                     DirTree::nDepth     () {  return _nDepth        ; }
} // End of Namespace wcb
