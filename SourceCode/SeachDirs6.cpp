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
#include "DirTree.hpp"
#include "SearchDirs6.hpp"
#include "SearchForStringFcns.hpp"
#include "parseTokens.hpp"

using namespace std;

int main(int argc,   char * const argv[])
{//string topDirName("..\\..\\..\\Projects\\");

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

   // Print directory tree.
   dirTree1.printDirTree();
   if(true) return 0;

   // Find all files (and their respective directory paths) with designated extensions in directory tree.
   vector<string> foundSubDirs, foundFiles;
   search4Files(dirTree1.parentDirNames(), dirTree1.filesV(), extensions, foundSubDirs, foundFiles);

   // Get lists of subdirectories, files, and lines containing the designated
   // search string, e.g. all files containing "#include"
   vector<string> subDirsMatched, filesMatched, pathNames, linesMatched;
   findStringInFiles(searchPatterns, foundSubDirs  , foundFiles,
                                     subDirsMatched, filesMatched, linesMatched);

   // Get the include file names and print them with the corresponding
   // pathnames of the files contaning them.
   size_t nlines = linesMatched.size();
   vector<string> filesOfInterest;
   for(size_t iline=0; iline<nlines; iline++)
   {  vector<string> tokens = parseTokens(linesMatched[iline], " \"<>" );
      printf("%s, %s, %s, %s\n", subDirsMatched[iline].c_str(),
                                 filesMatched[iline].c_str()  ,
                                 tokens[1].c_str()            ,
                                 linesMatched[iline].c_str()   );
      filesOfInterest.push_back(tokens[1]);
   }
   printf("\n");

   // Find all the referenced include files contained in the directory tree
   // and print their full pathnames.
   printf("Referenced include files found in %s:\n", topDirName.c_str());
   printf("Referenced Header File, Header File Subdirectory, Header File Name, Header File Pathname\n");
   vector<string> oneFile;
   int nfoi    = filesOfInterest.size();
   int nffiles = foundFiles     .size();
   for (int ifoi=0; ifoi<nfoi; ifoi++)
   {  int nfound = 0;

      string foi(filesOfInterest[ifoi]);
      printf("%s ", foi.c_str());
      
      // Convert to all uppercase for comparison.
      string foiUpper(foi);
      transform(foi.begin(), foi.end(), foiUpper.begin(), ::toupper);  
      for(int iffile=0; iffile<nffiles; iffile++)
      {  // Convert to all uppercase for comparison.
         string ffileUpper(foundFiles[iffile]);
         transform(foundFiles[iffile].begin(),
                   foundFiles[iffile].end  (), ffileUpper.begin(), ::toupper);
         if(foiUpper.compare(ffileUpper) == 0)
         {  nfound++;
            printf(", %s, %s, %s%s\n", foundSubDirs[iffile].c_str(), foundFiles[iffile].c_str(),
                                       foundSubDirs[iffile].c_str(), foundFiles[iffile].c_str() );
         }
      } // End of for(iffile)
      if (nfound == 0)
      {  printf(", Not found in %s\n", topDirName.c_str());
      }
   } // End of for(ifoi)

   return 0;
} // End of main()


int search4Files(vector<string>          ParentDirs  ,
                 vector<vector<string>>  filesV      ,
                 vector<string>          extensions  ,
                 vector<string>         &foundParents,
                 vector<string>         &foundFiles   )
{  int nParentDirs = ParentDirs.size();
   vector<string> subDirs;
   vector<string> files  ;
   int nfilesFound = 0;

   // Make extensions all uppercase.
   for (std::vector<string>::iterator ite = extensions.begin() ; ite != extensions.end(); ++ite)
   {  transform(ite->begin(), ite->end(),ite->begin(), ::toupper);
   }

   // Loop through Parent subdirectories.
   for(int iParnt=0; iParnt<nParentDirs; ++iParnt)
   {  files   = filesV[iParnt];
      
      for (std::vector<string>::iterator itf = files.begin() ; itf != files.end(); ++itf)
      {  //First get just the file name.
         int lastSlashPos = itf->find_last_of("/\\");
         string fileName = itf->substr(lastSlashPos+1);
         {  // Check file extension.
            for(vector<string>::iterator ite = extensions.begin(); ite<extensions.end(); ++ite)
            {  size_t extLen = ite    ->size();
               size_t fnLen  = fileName.size();
               if(extLen > fnLen)
               {  break;
               }
               string fnTail = fileName.substr(fileName.size() - extLen);
               // Transform to upper case.
               transform(fnTail.begin(), fnTail.end(), fnTail.begin(), ::toupper);
               if(fnTail.compare(*ite) == 0)
               {  // Include this file.
                  foundParents.push_back(ParentDirs[iParnt].c_str());
                  foundFiles  .push_back(fileName);
                  nfilesFound++;
               }
            } // End of for(extension)
         } // End of else if (extenstion.size()
      } // End of for (files)
    //if(!noFileFound)  printf("\n");
   } // End of for (parentDirs)

   return nfilesFound;

} // End of search4Files()




