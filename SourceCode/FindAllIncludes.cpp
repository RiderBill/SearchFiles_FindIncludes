#include <cstdlib>
#include <cstdio>
#include <tchar.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <new>
#include <algorithm>  // transsform() , e.g. toupper
#include "ConditionalIncludes.hpp"
#include "sortedVector10.hpp"
#include "DirTree9.hpp"
#include "SearchDirs8.hpp"
#include "SearchForStringFcns8.hpp"
#include "parseTokens8.hpp"
vector<string> conditionAsPathNames(char * pathNames);

using namespace std;

int main(int argc,   char * const argv[])
{  FILE *outfp;
   char *outFileName;

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
         searchPaths.push_back(targPathName.substr(0, targPathName.find_last_of("\\")));
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

   // Find all files (and their respective directory paths) with designated extensions in directory tree.
   vector<string> targSubDirs, targFiles, endingsNotFound;
   size_t lastBackSlashPos = targPathName.find_last_of('\\');
   if (lastBackSlashPos < targPathName.npos) 
   {  targSubDirs.push_back(targPathName.substr(0, lastBackSlashPos+1) );
      targFiles  .push_back(targPathName.substr(   lastBackSlashPos+1) );
   }
   else
   {  targSubDirs.push_back(""          );
      targFiles  .push_back(targPathName);
   }
   vector<string> searchPatterns;
   searchPatterns.push_back("#include");

   // Get lines containing searchPatterns in the target file
   vector<string> tmpDirsWithString, tmpFilesWithString  , tmpLinesWithString;
   vector<string> subDirsWithString, filesWithString     , includeLines      ;
   findStringInFiles(searchPatterns, targSubDirs       , targFiles,
                                     tmpDirsWithString ,
                                     tmpFilesWithString,
                                     tmpLinesWithString             );
      // Exclude commented out or other spurious lines with the #include text.
   size_t nlines = tmpLinesWithString.size();
   for(size_t iline=0; iline<nlines; iline++)
   {  string tmp = tmpLinesWithString[iline];
      if (tmp[tmp.find_first_not_of(" \t")] == '#')
      {  includeLines     .push_back(tmpLinesWithString[iline]);
         subDirsWithString.push_back(tmpDirsWithString [iline]);
         filesWithString  .push_back(tmpFilesWithString[iline]);
      }
   }

   // Collect all of the interesting filenames, i.e. include files, in targFile
   nlines = includeLines.size();
   vector<string>  includeFileNames;
   for(size_t iline=0; iline<nlines; iline++)
   {  vector<string> tokens = parseTokens(includeLines[iline], " \"<>" );
      includeFileNames.push_back(tokens[1]);
   }

   // Print include file names found in the target file.
   outFileName = "TargetFileHeaderList.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  fprintf_s(outfp, "Include File Name, Line containing string\n");
      for(size_t iline=0; iline<nlines; iline++)
      {  fprintf_s(outfp, "%s,%s\n", includeFileNames[iline].c_str(),
                                     includeLines[iline].c_str() );
      }

      fclose(outfp);
   }

   // Find all the referenced include files contained in the directory tree(s)
   // and print them.
   vector<string> foundSubDirs, foundFiles, filesToFind = includeFileNames;
   vector<string> filesNotFound;
   int nFiles;
   outFileName = "ReferencedFiles.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  for(itop=0; itop<nTops; itop++)
      {  foundSubDirs .clear();
         foundFiles   .clear();
         filesNotFound.clear();
         dirTrees[itop].findFilesByName(filesToFind, foundSubDirs, foundFiles, filesNotFound);

         fprintf_s(outfp, "Files found in %s:\n", dirTrees[itop].topDirName().c_str());
         fprintf_s(outfp, "FileName, Subdirectory, File Pathname\n");
         int nFiles=foundFiles.size();
         for(int ifile=0; ifile<nFiles; ifile++)
         {  fprintf_s(outfp, "%s,%s,%s\n", foundFiles  [ifile].c_str(), 
                                           foundSubDirs[ifile].c_str(),
                                          (foundSubDirs[ifile]+foundFiles[ifile]).c_str());
         }
         fprintf_s(outfp, "\n");
         filesToFind = filesNotFound;
      }
      fprintf_s(outfp, "Files not found in %s", dirTrees[0].topDirName().c_str());
      for(itop=1; itop<nTops-1; itop++)
      {  fprintf(outfp, "; %s", dirTrees[itop].topDirName().c_str());
      }
      if (nTops > 1) fprintf(outfp, "; nor %s: ", dirTrees[nTops-1].topDirName().c_str());
      fprintf_s(outfp, "\n");

      fprintf_s(outfp, "FileName, Not Found\n");
      nFiles=filesNotFound.size();
      for(int ifile=0; ifile<nFiles; ifile++)
      {  fprintf_s(outfp, ", %s, , Not Found\n", filesNotFound  [ifile].c_str() );
      }
      fclose(outfp);
   }
         
      return 0;
} // End of main()


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
