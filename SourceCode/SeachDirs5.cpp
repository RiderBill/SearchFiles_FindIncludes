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

#include "SearchDirs5.hpp"
#include "SearchForStringFcns.hpp"
#include "parseTokens.hpp"

using namespace std;

#if (defined USERDATABILL)
   #include <U:\\Bill\\LANG\\VCC_2012\\Brin_Inc\\fcn_s_MACROS.h>
   #if (defined(_MSC_VER))
      #include <U:\\Bill\\LANG\\dirent_include\\dirent.h>
   #else
     #include <dirent.h>
   #endif // if(defined(_MSC_VER)
#else
   #include <C:\\Users\\brinson\\Documents\\Lang\\fcn_s_MACROS.h>
   #if (defined(_MSC_VER))
      #include <C:\\Users\\brinson\\Documents\\Lang\\dirent.h>
   #else
     #include <dirent.h>
   #endif // if(defined(_MSC_VER)
#endif // if/else (defined USERDATABILL)

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

   vector<string>         topDirNames;
   vector<vector<string>> subDirs;
   vector<vector<string>> files;
   getDirTree  (topDirName, 0, topDirNames, subDirs, files);
   printDirTree(            0, topDirNames, subDirs, files);

   vector<string> foundSubDirs, foundFiles;
   search4Files( topDirNames, files, extensions, foundSubDirs, foundFiles);

   vector<string> subDirsMatched, filesMatched, pathNames, linesMatched;
   findStringInFiles(searchPatterns, foundSubDirs  , foundFiles,
                                     subDirsMatched, filesMatched, linesMatched);

   size_t nlines = linesMatched.size();
   vector<string> filesOfInterest;
   for(size_t iline=0; iline<nlines; iline++)
   {  vector<string> tokens = parseTokens(linesMatched[iline], " \"<>" );
      printf("%s, %s, %s, %s\n", subDirsMatched[iline].c_str(), filesMatched[iline].c_str(), tokens[1].c_str(), linesMatched[iline].c_str());
      filesOfInterest.push_back(tokens[1]);
   }

   printf("\n");
   printf("Referenced include files found in %s:\n", topDirName.c_str());
   vector<string> oneFile;
   int nfoi    = filesOfInterest.size();
   int nffiles = foundFiles     .size();
   for (int ifoi=0; ifoi<nfoi; ifoi++)
   {  int nfound = 0;

      string foi(filesOfInterest[ifoi]);
      printf("%s: ", foi.c_str());
      
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

   //for(vector<string>::iterator iFOI=filesOfInterest.begin();
   //                             iFOI<filesOfInterest.end(); iFOI++)
   // foundSubDirs.clear();
   // foundFiles  .clear();
   // oneFile     .clear();
   // oneFile.push_back(filesOfInterest[ifoi]); // Strangly I find myself needing a vector with
   //                                           // only one value.
   // int nfound = search4Files(topDirNames, files, oneFile, foundSubDirs, foundFiles);
   // if (nfound > 0)
   // {  for(int ifound=0; ifound<nfound; ifound++)
   //    {  printf(", %s, %s, %s%s\n", foundSubDirs[ifound].c_str(), foundFiles[ifound].c_str(),
   //                                  foundSubDirs[ifound].c_str(), foundFiles[ifound].c_str() );
   //    }
   // }
   // else
   // {  printf(", Not found in %s\n", topDirName.c_str());
   // }

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



int getDirTree  (string topDirName,  int maxDepth,
                 vector<string>         &topDirNames,
                 vector<vector<string>> &subDirsV,
                 vector<vector<string>> &filesV       )
{  int status = 0;
   struct dirent *drnt;
   DIR *dr;
   vector<string> subDirs;
   vector<string> files;

   dr=opendir(topDirName.c_str());
   if (dr!=NULL)
   {  topDirNames.push_back(topDirName);
      while ((drnt=readdir(dr)) != NULL)
      {  string fdName(drnt->d_name);
         if (  (fdName == "." )
             ||(fdName == "..") ) continue;
         fdName = topDirName + fdName;

         if ( drnt->d_type == DT_DIR)
         {  string subDirName = fdName + "\\";
            subDirs.push_back(subDirName);
         }
         else
         {  files.push_back(fdName);
         }
      } // End of while(drnt!=NULL
      closedir(dr);

      subDirsV.push_back(subDirs);
      filesV  .push_back(files  );

      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  getDirTree(*it, maxDepth+1, topDirNames, subDirsV, filesV);
      }
   } // End of if(dr!=NULL)
   else
   {  printf("Unable to open %s as a directory\n", topDirName);
      status = 1;
   }  // End of else if(dr!=NULL)
 
   return status;
} // End of getDirTree()


int printDirTree(                    int maxDepth,
                 vector<string>          topDirNames,
                 vector<vector<string>>  subDirsV,
                 vector<vector<string>>  filesV      )
{  int nTopDirNames = topDirNames.size();
   vector<string> subDirs;
   vector<string> files  ;

   for(int iTop=0; iTop<nTopDirNames; ++iTop)
   {  subDirs = subDirsV[iTop];
      files   = filesV  [iTop];
      printf("Contents of %s:\n", topDirNames[iTop].c_str());
      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  printf("            %s\n", it->c_str());
      }
      for (std::vector<string>::iterator it = files.begin() ; it != files.end(); ++it)
      {  printf("            %s\n", it->c_str());
      }
      printf("\n");
   }

   return 0;
} // End of getDirTree()

