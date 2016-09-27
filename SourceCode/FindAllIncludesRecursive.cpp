#include <cstdlib>
#include <cstdio>
#include <tchar.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <set>
#include <map>
//#include <new>
#include <algorithm>  // transsform() , e.g. toupper
#include "ConditionalIncludes.hpp"
#include "DirTree11.hpp"
#include "SearchDirs8.hpp"
#include "SearchForStringFcns8.hpp"
#include "parseTokens8.hpp"
#include "sortedVector10.hpp"
vector<string> conditionAsPathNames(char * pathNames);

using namespace std;
//#############################################################################################################
int findIncludeFiles(string const    targPathName  , vector<wcb::DirTree> const dirTrees   ,
                     set<string>    &uniqueIncludes, map<string, set<string>> &incMap      , 
                     vector<string> &foundIncludes , vector         <string>  &foundPaths  ,
                     vector<string> &includeLines  , set            <string>  &foundDirsSet,
                     set   <string> &foundFilesSet , set<string>    &filesNotFoundSet        );

void printResults(vector<wcb::DirTree> dirTrees                         ,
                  std::map<string, set<string>> incMap                  ,
                  set<string> foundDirsSet, set<string> filesNotFoundSet );
//#############################################################################################################
int main(int argc,   char * const argv[])
{  
   // The following are to simplify for debugging.
   char *argv0 = argv[0];
   char *argv1 = argv[1];
   char *argv2 = argv[2];
   char *argv3 = argv[3];
   char *argv4 = argv[4];
   char *argv5 = argv[5];

   if(argc < 3)
   {  printf("Enter source file and one or more search directories\n"
             "on command line.\n"
             "Enclose directory paths containing spaces in double quotes\n");
      printf("Program termites.\n");
      exit (-1);
   }

    // Parse command line directory arguments, strip off leading and
   // trailing quotes, leading and trailing white space,
   // and add trailing backslashes.
   vector<string> pathNames, searchPaths;
   string targPathName;
   for(int iarg=1; iarg<argc; iarg++)
   {  pathNames = conditionAsPathNames(argv[iarg]);
      if (iarg == 1)
      {  targPathName = pathNames[0];
  //     searchPaths.push_back(targPathName.substr(0, targPathName.find_last_of("\\")));
      }
      else
      {  for(vector<string>::iterator ipath=pathNames.begin();
                                      ipath<pathNames.end(); ipath++)
         {  searchPaths.push_back(*ipath);
         }
      }
   }

   // Construct directory Tree objects.
   vector<wcb::DirTree> dirTrees   ;
   int itop, nTops = searchPaths.size();
   for(itop=0; itop<nTops; itop++)
   {  dirTrees   .push_back(wcb::DirTree(searchPaths[itop]));
   }

   vector<string> foundIncludes, foundPaths, includeLines;
   set   <string>              uniqueIncludeSet ;
   set   <string>              oldIncludeSet    ;
   set   <string>              foundDirsSet     ;
   set   <string>              foundFilesSet    ;
   set   <string>              filesNotFoundSet ;
   set   <string>              filesToSearchSet ;
   map   <string, set<string>> incMap           ;
 //findIncludeFiles(targPathName , dirTrees     , uniqueIncludeSet,
 //                 incMap       , foundIncludes, foundPaths      ,
 //                 includeLines , foundDirsSet , foundFilesSet   ,
 //                 filesNotFoundSet                               );
   filesToSearchSet.insert(targPathName);
   foundFilesSet   .insert(targPathName);
// filesToSearchSet = foundFilesSet;
// filesToSearchSet = uniqueIncludeSet;
   do
   {  oldIncludeSet = uniqueIncludeSet;
    //foundFilesSet.clear();
      for(set<string>::iterator it = filesToSearchSet.begin();
                                it!= filesToSearchSet.end  (); ++it)
      {  targPathName = *it;
       //if(foundFilesSet.find(targPathName)!=foundFilesSet.end())
         {    // targPathName has been found.  Look for include files
              // inside it.
            findIncludeFiles(targPathName , dirTrees     , uniqueIncludeSet,
                             incMap       , foundIncludes, foundPaths      ,
                             includeLines , foundDirsSet , foundFilesSet   ,
                             filesNotFoundSet                               );
            
         }
      }
      filesToSearchSet = foundFilesSet;
//    filesToSearchSet = uniqueIncludeSet;
      //Stop when no more new files are found.
   } while( oldIncludeSet.size() < uniqueIncludeSet.size() );


   //////////////////////////////////////////////////////////////////////////////////
   //for(set<string>::iterator it = uniqueIncludeSet.begin();
   //                          it!= uniqueIncludeSet.end  (); ++it)
   //{  targPathName = *it;
   //   if(foundFilesSet.find(targPathName)!=foundFilesSet.end())
   //   {    // targPathName has been found.  Look for include files
   //        // inside it.
   //      findIncludeFiles(targPathName , dirTrees     , uniqueIncludeSet,
   //                       incMap       , foundIncludes, foundPaths      ,
   //                       includeLines , foundDirsSet , foundFilesSet   ,
   //                       filesNotFoundSet                               );
   //   }
   //}
   //
   //for(set<string>::iterator it = filesNotFoundSet.begin();
   //                          it!= filesNotFoundSet.end  (); ++it)
   //{  targPathName = *it;
   //   if(foundFilesSet.find(targPathName)!=foundFilesSet.end())
   //   {    // targPathName has been found.  Look for include files
   //        // inside it.
   //      findIncludeFiles(targPathName , dirTrees     , uniqueIncludeSet,
   //                       incMap       , foundIncludes, foundPaths      ,
   //                       includeLines , foundDirsSet , foundFilesSet   ,
   //                       filesNotFoundSet                               );
   //   }
   //}

   printResults(dirTrees, incMap, foundDirsSet, filesNotFoundSet );

} // End of main()

//#############################################################################################################

int findIncludeFiles(string const    targPathName  , vector<wcb::DirTree> const dirTrees   ,
                     set<string>    &uniqueIncludes, map<string, set<string>> &incMap      , 
                     vector<string> &foundIncludes , vector         <string>  &foundPaths  ,
                     vector<string> &includeLines  , set            <string>  &foundDirsSet,
                     set   <string> &foundFilesSet , set            <string>    &filesNotFoundSet)
{        
     // Split targPathname into path and filename.
     // findStringInFiles takes vectors of paths and filenames, so
     // create the vectors with single strings in each.
   string targName, targDir;
   size_t lastSlash = targPathName.find_last_of("/\\");
   if (lastSlash < targPathName.npos)
   {  targName = targPathName.substr(   lastSlash+1);
      targDir  = targPathName.substr(0, lastSlash  );
   }
   else
   {  targName = targPathName;
      targDir  = ""          ;
   }

     // Pattern to identify potential include lines in file(s).
   vector<string> searchPatterns;
   searchPatterns.push_back("#include");

     // Find all occurrances of the target file in the dirTrees
   set            <string>  foiSet;
   vector         <string>  foundDirsV   , foundFilesV  ;
   map<string, set<string>> foundfilesMap;

   foiSet.insert(targName);

   int itop, nTops = dirTrees.size();
   for(itop=0; itop<nTops; itop++)
   {  dirTrees[itop].findFilesByName(foiSet, foundDirsV      , foundFilesV ,
                                             filesNotFoundSet, foundDirsSet, 
                                             foundFilesSet   , incMap        );

         // Find all referenced include files in targPathName file.
         // First get lines containing searchPatterns in the target file
      vector<string> tmpDirsWithString, tmpFilesWithString, tmpLinesWithString;
      vector<string> includeFileNames;
      tmpDirsWithString .clear();
      tmpFilesWithString.clear();
      tmpLinesWithString.clear();
//    includeFileNames  .clear();
//    foiSet            .clear();
      findStringInFiles(searchPatterns   , foundDirsV        , foundFilesV       ,
                        tmpDirsWithString, tmpFilesWithString, tmpLinesWithString );

         // Exclude commented out or other spurious lines with the #include text.
      vector<string> subDirsWithString, filesWithString      ;
      size_t nlines = tmpLinesWithString.size();
      for(size_t iline=0; iline<nlines; iline++)
      {  string tmp = tmpLinesWithString[iline];
         if (tmp[tmp.find_first_not_of(" \t")] == '#')
         {     // Nothing but whitespace preceeding #.  Must be an include directive.
            includeLines     .push_back(tmpLinesWithString[iline]);
            subDirsWithString.push_back(tmpDirsWithString [iline]);
            filesWithString  .push_back(tmpFilesWithString[iline]);

            // Collect all of the interesting filenames, i.e. include files,
            // in includeFileNames
            vector<string> tokens = parseTokens(tmpLinesWithString[iline], " \t\"<>" );
   if(strncmp(tokens[1].c_str(),"bits", 4)== 0)
   {  printf("tokens[1].c_str() = %s\n", tokens[1].c_str());
   }
   lastSlash = tokens[1].find_last_of("/\\");
   if (lastSlash < tokens[1].npos)
   {  targName =   tokens[1].substr(   lastSlash+1);
   }               
   else            
   {  targName =   tokens[1];
   }
 
            includeFileNames.push_back(targName);
            if(uniqueIncludes.emplace (targName).second)
            {  foiSet        .emplace (targName);
            }
            
         } // end of if(first char is #)
      } // End of for(iline)

   } // End of 1st for(iDirTree)

     // Update filesNotFoundSet and remove files already found from foiSet
   set<string>::iterator itff, itnf, itfoi;
   for(itff=foundFilesSet.begin(); itff!=foundFilesSet.end(); itff++)
   {    // Remove files found from filesNotFoundSet
      if((itnf = filesNotFoundSet.find(*itff))!=filesNotFoundSet.end())
      {  filesNotFoundSet.erase(*itnf);
      }
        // Remove files found from foiSet
      if((itfoi = foiSet.find(*itff))!=foiSet.end())
      {  foiSet.erase(*itfoi);
      }
   }

     // Find the newly identified include files that have not already been found
   for(itop=0; itop<nTops; itop++)
   {  dirTrees[itop].findFilesByName(foiSet, foundPaths      , foundIncludes,
                                     filesNotFoundSet, foundDirsSet , 
                                     foundFilesSet   , incMap         );
   } // End of 2nd for(iDirTree)

   return 0;
} // End of FindIncludeFile()


vector<string> conditionAsPathNames(char * str)
{  // Parse str into pathNames, strip off leading and
   // trailing quotes, leading and trailing white space,
   // and add trailing backslashes.

   vector<string> tmp = parseTokens(string(str), ";\"" );
   vector<string> pathNames;
   for(size_t itmp=0; itmp<tmp.size(); itmp++)
   {  int firstChar = tmp[itmp].find_first_not_of("\" \t\n");
      int lastChar  = tmp[itmp].find_last_not_of ("\" \t\n");
      
      // Either both or neither of firstChar and lastChar==string::npos
      if (firstChar == string::npos)
      {  continue;
      }
      else
      {  tmp[itmp]=tmp[itmp].substr(firstChar    );
         tmp[itmp]=tmp[itmp].substr(0, lastChar+1);
         pathNames.push_back(tmp[itmp]);
      } // End of if/else(firstChar == string.npos)
   } // End of for(itmp)
   return tmp;
} // End of conditionAsPathNames()

//#############################################################################################################

void printResults(vector<wcb::DirTree> dirTrees                         ,
                  std::map<string, set<string>> incMap                  ,
                  set<string> foundDirsSet, set<string> filesNotFoundSet )
{  FILE *outfp;
   char *outFileName;

     // Print all directly and indirectly referenced include file names and
     // the directory(ies) where they were found.
     // Also print unfound include filenames.
   outFileName = "incMap.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  fprintf_s(outfp, "Include File Name, Path(s) where found\n");
      map<string, set<string>>::iterator itm;
      pair<string, set<string>> apair;
      for(itm=incMap.begin(); itm!=incMap.end(); ++itm)
      {  apair = *itm;
         fprintf_s(outfp, "%s", apair.first.c_str());
         set<string>::iterator its;
         for(its=apair.second.begin(); its!=apair.second.end(); ++its)
         {  fprintf_s(outfp, ",%s\n", its->c_str());
         }
      }
      set<string>::iterator its;
      for(its=filesNotFoundSet.begin(); its!=filesNotFoundSet.end(); ++its)
      {  fprintf_s(outfp, "%s, Not found\n", its->c_str());
      }

      fclose(outfp);
   }

     // Print a list of unique directory(ies) where include files were found.
   outFileName = "foundPaths.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  size_t nfiles = foundDirsSet.size();
      fprintf_s(outfp, "Include file Path(s)\n");
      set<string>::iterator its;
      for(its=foundDirsSet.begin(); its!=foundDirsSet.end(); ++its)
      {  fprintf_s(outfp, "%s\n", its->c_str());
      }
      fclose(outfp);
   }

     // Print directory trees.
   outFileName = "DirectoryTrees.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  size_t nDirs = dirTrees.size();
      for(size_t iDir=0; iDir<nDirs; iDir++)
      {  dirTrees[iDir].printDirTree(outfp, 1);
      }
   }

   return;
} // End of printResults()

