#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <limits.h>     // defines PATH_MAX
#include <new>

#include <C:\\Users\\brinson\\Documents\\Lang\\fcn_s_MACROS.h>

#if (defined(_MSC_VER))
  #include <C:\\Users\\brinson\\Documents\\Lang\\dirent.h>
#else
  #include <dirent.h>
#endif // if(defined(_MSC_VER)

using namespace std;

int getDirTree  (string topDirName,  int maxDepth,
                 vector<string>         &topDirNames,
                 vector<vector<string>> &subDirsV,
                 vector<vector<string>> &filesV      );
int printDirTree(                    int maxDepth,
                 vector<string>          topDirNames,
                 vector<vector<string>>  subDirsV,
                 vector<vector<string>>  filesV      );
vector<string> parseTokens(string line1, char *delimeters);
int search4Files(vector<string>          ParentDirs  ,
                 vector<vector<string>>  filesV      ,
                 vector<string>          extensions  ,
                 vector<string>         &foundParents,
                 vector<string>         &foundFiles   );



int main(int argc,  const char*  argv[])
{//string topDirName("..\\..\\..\\Projects\\");

   if(argc < 4)
   {  printf("Enter starting directory, file extention types, and search text\n"
             "on command line.\n"
             "Enter "" for all files.\n");
      printf("Program termites.\n");
      exit (-1);
   }

   string topDirName    (argv[1]);
   string fileExtensions(argv[2]);
   string searchPattern (argv[3]);
   vector<string> extensions = parseTokens(fileExtensions, ", ;");

   vector<string>         topDirNames;
   vector<vector<string>> subDirs;
   vector<vector<string>> files;
   getDirTree  (topDirName, 0, topDirNames, subDirs, files);
 //printDirTree(            0, topDirNames, subDirs, files);

   vector<string> foundSubDirs, foundFiles;
   search4Files( topDirNames, files, extensions, foundSubDirs, foundFiles);

   return 0;
} // End of main()


vector<string> parseTokens(string line1, char *delimeters)
{  // First make a copy of the line so as to not obliterate the original.
   unsigned int sourceLength = line1.size();
   unsigned int destLength   = sourceLength + 1; // Leave room for terminating null
 //char *line = new (char) (destLength)  ;
   char *line = new char [destLength]  ;
   char *cptr, *nextTok = NULL;
   vector<string> tokens;

   strncpy_s(line, destLength, line1.c_str(), sourceLength);

   cptr = strtok_s(line, delimeters, &nextTok);
   tokens.push_back(cptr);
   while ( (cptr = strtok_s(NULL, delimeters, &nextTok)) != NULL)
   {  tokens.push_back(cptr);
   }
   delete[] (line);
   return tokens;
} // End of parseTokens()


#include <algorithm>
int search4Files(vector<string>          ParentDirs  ,
                 vector<vector<string>>  filesV      ,
                 vector<string>          extensions  ,
                 vector<string>         &foundParents,
                 vector<string>         &foundFiles   )
{  int nParentDirs = ParentDirs.size();
   vector<string> subDirs;
   vector<string> files  ;

   // Make extensions all uppercase.
   for (std::vector<string>::iterator ite = extensions.begin() ; ite != extensions.end(); ++ite)
   {  transform(ite->begin(), ite->end(),ite->begin(), ::toupper);
   }

   // Loop through Parent subdirectories.
   for(int iParnt=0; iParnt<nParentDirs; ++iParnt)
   {  bool noFileFound = true;
      files   = filesV[iParnt];
      
      for (std::vector<string>::iterator itf = files.begin() ; itf != files.end(); ++itf)
      {  //First get just the file name.
         int lastSlashPos = itf->find_last_of("/\\");
         string fileName = itf->substr(lastSlashPos+1);
       //if(extensions.size() == 0)
       //{  // Include all files.
       //   foundParents.push_back(ParentDirs[iParnt].c_str());
       //   foundFiles  .push_back(fileName);
       //} // End of if (extenstion.size()
       //else
         {  // Check file extension.
            for(vector<string>::iterator ite = extensions.begin(); ite<extensions.end(); ++ite)
            {  size_t extLen = ite->size();
               size_t fNLen  = itf->size();
               string fnTail = fileName.substr(fileName.size() - extLen);
               // Transform to upper case.
               transform(fnTail.begin(), fnTail.end(), fnTail.begin(), ::toupper);
               if(fnTail.compare(*ite) == 0)
               {  // Include this file.
                  foundParents.push_back(ParentDirs[iParnt].c_str());
                  foundFiles  .push_back(fileName);
                  if(noFileFound)
                  {  noFileFound = false;
                     printf("===================================================\n");
                     printf("%s:\n", ParentDirs[iParnt].c_str());
                  }
                  printf("%s, %s\n", ParentDirs[iParnt].c_str(), fileName.c_str());
               }
            } // End of for(extension)
         } // End of else if (extenstion.size()
      } // End of for (files)
      if(!noFileFound)  printf("\n");
   } // End of for (parentDirs)

   return 0;

} // End of searchFiles()



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


//class wcbDirectory
//{  public:
//
//   // Constructors
//   wcbDirectory(char *dirName)
//   {  
//
//   }
//
//   private:
//   ~wcbDirectory()
//   {
//   }
//
//   int nSubDirs;
//   int nRegularFiles;
//  
//   array string = string(subDirNames[];
//   string fileNames  [];
//
//
//}
//
//} // End of class wcbDirectory

