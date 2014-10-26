#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>  // strerror()
#include <vector>
#include <limits.h>     // defines PATH_MAX
#include <C:\\Users\\brinson\\Documents\\Lang\\fcn_s_MACROS.h>

#if (defined(_MSC_VER))
  #include <C:\\Users\\brinson\\Documents\\Lang\\dirent.h>
#else
  #include <dirent.h>
#endif // if(defined(_MSC_VER)

using namespace std;
int parseDir(string topDirName, int idepth);
int parseDir(string topDirName, int idepth, vector<string> &subDirs, vector<string> &files);
int list_dir(const char * dir_name);


int main(int argc,  const char*  argv[])
{  string topDirName("..\\..\\..\\Projects\\");
   vector<string> subDirs;
   vector<string> files;

   parseDir(topDirName, 0, subDirs, files);

   return 0;
} // End of main()

int parseDir(string topDirName, int idepth, vector<string> &subDirs, vector<string> &files)
{  struct dirent *drnt;
   DIR *dr;

   dr=opendir(topDirName.c_str());
   if (dr!=NULL)
   {  while ((drnt=readdir(dr)) != NULL)
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

      printf(   "Contents of %s:\n", topDirName.c_str());
      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  printf("            %s\n", (*it).c_str());
      }
      for (std::vector<string>::iterator it = files.begin() ; it != files.end(); ++it)
      {  printf("            %s\n", (*it).c_str());
      }
      printf("\n");
      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  parseDir(*it, idepth+1);
      }
   } // End of if(dr!=NULL)
   else
   {  printf("Unable to open %s as a directory\n", topDirName);
   }  // End of else if(dr!=NULL)
 
   return 0;
} // End of parseDir()

int parseDir(string topDirName, int idepth)
{  struct dirent *drnt;
   DIR *dr;
   vector<string> subDirs;
   vector<string> files;

   dr=opendir(topDirName.c_str());
   if (dr!=NULL)
   {  while ((drnt=readdir(dr)) != NULL)
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

         printf("Contents of %s:\n", topDirName.c_str());
      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  printf("            %s\n", (*it).c_str());
      }
      for (std::vector<string>::iterator it = files.begin() ; it != files.end(); ++it)
      {  printf("            %s\n", (*it).c_str());
      }
      printf("\n");
      for (std::vector<string>::iterator it = subDirs.begin() ; it != subDirs.end(); ++it)
      {  parseDir(*it, idepth+1);
      }
   } // End of if(dr!=NULL)
   else
   {  printf("Unable to open %s as a directory\n", topDirName);
   }  // End of else if(dr!=NULL)
 
   return 0;
} // End of parseDir()


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

