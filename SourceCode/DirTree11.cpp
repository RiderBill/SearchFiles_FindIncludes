#include <cstdlib>
#include <cstdio>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>      // strerror()
#include <vector>
#include <algorithm>    // transsform() , e.g. toupper
#include "sortedVector10.hpp"
#include "ConditionalIncludes.hpp"
#include "DirTree11.hpp"

using namespace std;

namespace wcb
{  // class DirTree
   // Constructor
   DirTree::DirTree(std::string topDirName1, int maxDepth1) :
      _topDirName(topDirName1),
      _maxDepth  (maxDepth1  ),
      _nDepth    (0          )
   {  int iDepth = 0;
      _parentDirNames.clear();
      _parentDepths  .clear();
      _childDirsV    .clear();
      _childFilesV   .clear();
      _childDirsM    .clear();
      _childFilesM   .clear();

      mapDirTree(_topDirName, iDepth);
   } // End of DirTree(std::string &topDirName1, int maxDepth1=0) constructor


   int DirTree::mapDirTree  (string parentName, int iDepth)
   {  int status;
      struct dirent *drnt;
      DIR *dr;
      vector<string> subDirs; // Subdirectories just in this parent directory.
      vector<string> files;   // Files just in this parent directory.
   
      if(_nDepth == 0)
      {  _parentDirNames.clear();
         _parentDepths  .clear();
         _childDirsV    .clear();
         _childFilesV   .clear();
         _childDirsM    .clear();
         _childFilesM  . clear();
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
               wcb::insertIntoVector(subDirs, subDirName);  // Alphabetical order
              _childDirsM[drnt->d_name].insert(parentName);
            }
            else
            {  // File
               string fileName   = fdName;
               wcb::insertIntoVector(files  , fileName  );  // Alphabetical order
              _childFilesM[drnt->d_name].insert(parentName);
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
   } // End of mapDirTree()
   
   
   int DirTree::printDirTree(FILE *outfp, int i) const
   {  int nParentDirNames = _parentDirNames.size();
      string shortName;
      for(int iParent=0; iParent<nParentDirNames; ++iParent)
      {  fprintf_s(outfp, "Contents of %s:\n", _parentDirNames[iParent].c_str());
         for (vector<string>::const_iterator it  = _childDirsV[iParent].begin();
                                             it != _childDirsV[iParent].end()  ;
                                           ++it)
         {  size_t lastSlash = it->substr(0, it->size()-1).find_last_of('\\');
            if (lastSlash < it->npos)
            {  shortName = it->substr(   lastSlash+1); // Dir
            }
            else
            {  shortName = *it;
            }

            fprintf_s(outfp, "            %s\n", shortName.c_str());
         }
         for (vector<string>::const_iterator it  = _childFilesV[iParent].begin();
                                             it != _childFilesV[iParent].end()  ;
                                           ++it)
         {  size_t lastSlash = it->find_last_of('\\');
            if (lastSlash < it->npos)
            {  shortName = it->substr(   lastSlash+1); // File
            }
            else
            {  shortName = *it;
            }

            fprintf_s(outfp, "            %s\n", shortName.c_str());
         }      }

      return 0;
   } // End of printDirTree()

   int DirTree::printDirTree(FILE *outfp) const
   {  int nParentDirNames = _parentDirNames.size();
   
      for(int iParent=0; iParent<nParentDirNames; ++iParent)
      {  fprintf_s(outfp, "Contents of %s:\n", _parentDirNames[iParent].c_str());
         for (vector<string>::const_iterator it  = _childDirsV[iParent].begin();
                                             it != _childDirsV[iParent].end()  ;
                                           ++it)
         {  fprintf_s(outfp, "            %s\n", it->c_str());
         }
         for (vector<string>::const_iterator it  = _childFilesV[iParent].begin();
                                             it != _childFilesV[iParent].end()  ;
                                           ++it)
         {  fprintf_s(outfp, "            %s\n", it->c_str());
         }
      }

      return 0;
   } // End of printDirTree()


   
   int DirTree::findFilesByName    (set   <string>  filesOfInterest,
                                    vector<string>  &foundDirsV    ,
                                    vector<string>  &foundFilesV   ,
                                    set   <string>  &filesNotFound ,
                                    set   <string>  &foundDirsSet  ,
                                    set   <string>  &foundFilesSet ,
                        map<string, set   <string>> &foundfilesMap  ) const
   /////////////////////////////////////////////////////////////////////////////
   // This function finds the filesOfInterest in the DirTree object
   // and returns the names and their paths in vector and map form.
   // Input:
   //  filesOfInterest - set of filenames to find w/o paths.
   // Output:
   //  foundDirsV      - vector of paths to found filenames
   //  foundFilesV     - vector of found filenames (with duplicates)
   //                    Note: one to one correspondence of foundDirsV
   //                          with foundFilesV (retained for compatibility
   //                          with older versions).
   //  filesNotFound   - set containing filenames not found in DirTree object
   //  foundFilesSet   - set of found filenames (all unique of course)
   //  foundFilesMap   - map of found filenames (key) and set of paths
   //                    the filenames were found in.
   /////////////////////////////////////////////////////////////////////////////
   {  int ifound = 0;
    //foundDirsV   .clear();
    //foundFilesV  .clear();
    //filesNotFound.clear();
    //foundDirsSet .clear();
    //foundFilesSet.clear();
    //foundfilesMap.clear();

      std::            set<string> ::iterator itFoi;
      std::map<char  , int        >::iterator it2;
      std::map<string, set<string>>::const_iterator itmap;

      for(itFoi=filesOfInterest.begin(); itFoi!=filesOfInterest.end(); ++itFoi)
      {  if ((itmap = _childFilesM.find(*itFoi)) != _childFilesM.end())
         {  ifound++;
            foundFilesSet.insert   (*itFoi);
            foundfilesMap.insert   (*itmap);
            set<string > tempS;
            tempS = (*itmap).second;
            set<string>::iterator itrS;
            for(itrS=tempS.begin(); itrS!=tempS.end(); ++itrS)
            {  foundDirsV   .push_back(*itrS );
               foundDirsSet .insert   (*itrS );
               foundFilesV  .push_back(*itFoi);
            }
         }
         else
         {  filesNotFound.insert   (*itFoi);
         }
      }
      return ifound;
   } // End of DirTree::findFilesByName (set   <string>  filesOfInterest,


   int DirTree::findFilesByName(vector<string> filesOfInterest,
                                vector<string> &foundDirs      ,
                                vector<string> &foundFiles     ,
                                vector<string> &filesNotFound   ) const
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
                    // Can't use the insertIntoVector function here.
                    // might find the file more than once, and more than one
                    // file might be in the same subdirectory.
                  foundFiles.push_back(fileName                );
                  foundDirs .push_back(_parentDirNames[iParent]);
               }
               
            } // End of for(iChild)
         } // End of for(iParent)
         if (nfound == 0)
         {  // Keep these in order and non-redundant.
            wcb::insertIntoVector(filesNotFound, filesOfInterest[ifoi]);
         }
      } // End of for(ifoi)

      return nFoundTotal;
   } // End of findFilesByName()
   
   
   int DirTree::findFilesEndingWith(vector<string>  endings        ,
                                    vector<string> &foundDirs      ,
                                    vector<string> &foundFiles     ,
                                    vector<string> &endingsNotFound ) const
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
                    // Can't use the insertIntoVector function here.
                    // might find the file more than once, and more than one
                    // file might be in the same subdirectory.
                  foundFiles.push_back(fileName                );
                  foundDirs .push_back(_parentDirNames[iParent]);
               }
               
            } // End of for(iChild)
         } // End of for(iParent)
         if (nfound == 0)
         {  // Keep these in order and non-redundant.
             wcb::insertIntoVector(endingsNotFound, endings[iEnd]);
         }
      } // End of for(iEnd)

      return nFoundTotal;
  } // End of findFilesEndingWith()

  string                  DirTree::topDirName     () const { return _topDirName    ;}
  vector<string>          DirTree::parentDirNames () const { return _parentDirNames;}
  vector<int   >          DirTree::parentDepths   () const { return _parentDepths  ;}
  vector<vector<string>>  DirTree::childDirsV     () const { return _childDirsV    ;}
  vector<vector<string>>  DirTree::childFilesV    () const { return _childFilesV   ;}
  map<string, set<string>>DirTree::childDirsM     () const { return _childDirsM    ;}
  map<string, set<string>>DirTree::childFilesM    () const { return _childFilesM   ;}
  int                     DirTree::nDepth         () const { return _nDepth        ;}
} // End of Namespace wcb DirTree::
