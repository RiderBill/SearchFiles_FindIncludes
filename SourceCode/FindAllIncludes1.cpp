#include <stdlib.h>
#include <stdio.h>
#include <TCHAR.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <new>
#include <algorithm>  // transsform() , e.g. toupper
#include "ConditionalIncludes.hpp"
#include "sortedVector8.hpp"
#include "DirTree8.hpp"
#include "SearchDirs8.hpp"
#include "SearchForStringFcns8.hpp"
#include "parseTokens8.hpp"

using namespace std;

int main(int argc,   char * const argv[])
{  FILE *outfp;
   char *outFileName;

   if(argc < 4)
   {  printf("Enter starting directory, file extention type(s), and search pattern(s)\n"
             "on command line.\n"
             "Enclose multiple extenstions or multiple search patterns in double quotes\n"
             "Enter "" for all files.\n"
             "(search pattern(s) cannot be a null string).\n");
      printf("Program termites.\n");
      exit (-1);
   }

   string topDirName    (argv[1]);
   string fileExtensions(argv[2]);
   vector<string> searchPatterns  = parseTokens(argv[3]       , " ,;"); //Space, comma, semicolon
   vector<string> extensions      = parseTokens(fileExtensions, ", ;");

   // Construct directory Tree object.
   wcb::DirTree dirTree1(topDirName);

   // Find all files (and their respective directory paths) with designated extensions in directory tree.
   vector<string> foundSubDirs, foundFiles, endingsNotFound;
   dirTree1.findFilesEndingWith(extensions, foundSubDirs, foundFiles, endingsNotFound);

   // Get lists of subdirectories, files, and lines containing the designated
   // search string, e.g. all files containing "#include"
   vector<string> subDirsWithString, filesWithString, linesWithString;
   findStringInFiles(searchPatterns, foundSubDirs     , foundFiles,
                                     subDirsWithString,
                                     filesWithString  ,
                                     linesWithString   );
   
   // Collect all of the interesting filenames
   size_t nlines = linesWithString.size();
   vector<string> filesOfInterest;
   for(size_t iline=0; iline<nlines; iline++)
   {  vector<string> tokens = parseTokens(linesWithString[iline], " \"<>" );
      filesOfInterest.push_back(tokens[1]);
   }

   // Print file names with the corresponding pathnames of the files containing them.
   outFileName = "CodeHeaderList.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  fprintf_s(outfp, "Parent Dir, File Name, String, Line containing string\n");
      for(size_t iline=0; iline<nlines; iline++)
      {  vector<string> tokens = parseTokens(linesWithString[iline], " \"<>" );
         filesOfInterest.push_back(tokens[1]);
         fprintf_s(outfp, "%s, %s, %s, %s\n", subDirsWithString[iline].c_str(),
                                           filesWithString  [iline].c_str(),
                                           tokens           [    1].c_str(),
                                           linesWithString  [iline].c_str() );
      }
      fclose(outfp);
   }

   // Find all the referenced include files contained in the directory tree
   foundSubDirs.clear();
   foundFiles  .clear();
   vector<string> filesNotFound;
   dirTree1.findFilesByName(filesOfInterest, foundSubDirs, foundFiles, filesNotFound);

   // Print referenced files.
   outFileName = "ReferencedFilesIncode.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  fprintf_s(outfp, "Files found in %s:\n", topDirName.c_str());
      fprintf_s(outfp, "FileName, Subdirectory, File Pathname\n");
      int nFiles=foundFiles.size();
      for(int ifile=0; ifile<nFiles; ifile++)
      {  fprintf_s(outfp, "%s,%s,%s\n", foundFiles  [ifile].c_str(), 
                              foundSubDirs[ifile].c_str(),
                             (foundSubDirs[ifile]+foundFiles[ifile]).c_str());
      }
      fclose(outfp);
   }

   // Look for unfound files in alternate directory tree.
   // Construct directory Tree object.
   string topDirName2("B:\\captain\\msn\\main\\AllHeaders\\opt\\");
   wcb::DirTree dirTree2(topDirName2);

   foundSubDirs.clear();
   foundFiles  .clear();
   vector<string> filesNotFound2;
   dirTree2.findFilesByName(filesNotFound, foundSubDirs, foundFiles, filesNotFound2);

   // Print referenced files.
   outFileName = "ReferencedFilesNotIncode.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  fprintf_s(outfp, "Files found in %s:\n", topDirName2.c_str());
      fprintf_s(outfp, "FileName, Subdirectory, File Pathname\n");
      int nFiles=foundFiles.size();
      for(int ifile=0; ifile<nFiles; ifile++)
      {  fprintf_s(outfp, "%s,%s,%s\n", foundFiles  [ifile].c_str(), 
                            foundSubDirs[ifile].c_str(),
                           (foundSubDirs[ifile]+foundFiles[ifile]).c_str());
      }

      fprintf_s(outfp, "Files not found in %s or %s:\n", topDirName.c_str(), topDirName2.c_str());
      fprintf_s(outfp, "FileName, Not Found\n");
      nFiles=filesNotFound2.size();
      for(int ifile=0; ifile<nFiles; ifile++)
      {  fprintf_s(outfp, ", %s, , Not Found\n", filesNotFound2  [ifile].c_str() );
      }
      fclose(outfp);
   }

   // Print directory trees.
   outFileName = "DirectoryTree.csv";
   fopen_s(&outfp, outFileName, "w");
   if (outfp == NULL)
   {  fprintf_s(stderr, "Unable to open %s for output\n", outFileName);
   }
   else
   {  dirTree1.printDirTree(outfp);
      fprintf_s(outfp, "\n\n");
      dirTree2.printDirTree(outfp);
      fclose(outfp);
   }

      return 0;
} // End of main()




