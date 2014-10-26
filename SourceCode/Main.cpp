#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <string>
using namespace std;

// print the content of a text file.
int main(int argc, char* const argv[])
{
// char *filename = argv[1];
   char const *filename = "test.txt";
   int status = 0;

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"arg[0]contents:\n";
   cout << "argv[0] = " << argv[0] <<"\n";

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"Executable from arg[0]:\n";
   // This gives the correct path to the executable
   // (not guaranteed to work on all systems).
   string executable(argv[0]);
   string path;
   size_t pathEnd = executable.find_last_of("/\\)");
   if( (pathEnd != executable.npos) && (pathEnd < executable.length()) )
   {  path=executable.substr(0, pathEnd+1);
      cout << "Path from argv[0] = " << path <<"\n";
   }
   else
   {  cout << "Failed to find \"/\" or \"\\\" in argv[0]\n";
      status += 1;
   }

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"System(dir) for run directory:\n";
   system("dir"); //<-- I see the directory content that I expect,
                  //    and the file (test.txt).
   cout<<"\n";

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"fopen_s method:\n";
   // This works, so I must be in the right directory.
   FILE *inptr;
   fopen_s(&inptr, filename, "rb");
   if(inptr == NULL)
   {  cout <<"fopen_s failed\n\n";
      status += 2;
   }
   else
   {  cout <<"fopen_s succeded\n";
      char line[50];
      fgets(line, 30, inptr);
      cout << "First line of " << filename <<" : \n" << line << "\n";
      fclose(inptr);
   }

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"string::open("<<filename<<") method:\n";
   // This doesn't work.  ifs.badbit gets set.
   std::ifstream ifs1;
   cout << "ifs1.open("<<filename<<", std::ios_base::in)\n";
   ifs1.open (filename, std::ios_base::in);
   if(ifs1.badbit)
   {  cout <<"ifs1.open failed method.\n";
      status += 4;
   }
   ifs1.close();

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"string::open("<<(path+string(filename)).c_str()<<") method:\n";
   // This doesn't work.  ifs.badbit gets set.
   //
   std::ifstream ifs2;
   cout << "ifs2.open("<<(path+string(filename)).c_str()<<", std::ios_base::in)\n";
   ifs2.open ((path+string(filename)).c_str(), std::ios_base::in);
   if(ifs2.badbit)
   {  cout <<"ifs2 failed with constructor method.\n";
      status += 8;
   }
   ifs2.close();

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"Dir(Executable path):\n";
   path = __FILE__; // gets source code path, include file name
                    
   path = path.substr(0,1+path.find_last_of('\\')); //removes file name
   path = string("\"") + path + string("\"");       // (Enclosing quotes required
                                                    // due to spaces in path)

   system((  string("dir ")   //<-- I see the file that I expect.
           + path).c_str());  // (I copied test.txt to this directory)

   // Add input file to path to make absolute PathName.
   string test = path.substr(path.length()-1);
   string absPathName =  path.substr(0, path.length()-1) // Remove trailing quote.
                       + string(filename)
                       +string("\"");                    // Add trailing quote.
   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"Dir(absolutPathName):\n";
   system((  string("dir ")
           + absPathName).c_str());   //<-- Again I see the file that I expect.

   cout<<"\n///////////////////////////////////////////////////\n";
   cout<<"string constructor with abs filename method:\n";
   cout << "ifs3.open("<<absPathName<<", std::ios_base_base::in)\n";
   // I also Tried this.  No luck.
   std::ifstream ifs3;
   string absPathName2 = absPathName.substr(1, absPathName.length()-2);
   ifs3.open(absPathName2, std::ios_base::in);
   if(ifs3.badbit)
   {  cout <<"ifs3.open failed with __FILE__ method\n";
      status += 16;
   }
   ifs3.close();

   cout<<"status:" << status <<"\n";
   return status; // Returns 0 from cmd line, 12 from MSVS
} // End of main()

//int main(int argc, char* argv[])
//{  std::ifstream  inDir;
//   std::ifstream  inFile;
//   std::ofstream  outFile;
//   char*          tempFileName;
//   const int      maxLineLength = 256;
//   char           line[maxLineLength];
//
//   test();
//
//   inDir  .exceptions ( std::ifstream::failbit | std::ifstream::badbit );
//   inFile .exceptions ( std::ifstream::failbit | std::ifstream::badbit );
//   outFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
//
//  std::ifstream ifs;
//
//  ifs.open ("doc1.txt", std::ifstream::in);
//
//  char c = ifs.get();
//
//   if(argc<4)
//   {  cout<<"Enter starting directory, output filename, and search string(s) on command linen";
//      cout<<"Program Termites";
//      return -1;
//   }
//   try
//   {//tempFileName = argv[1];
//      tempFileName = "doc1.txt";
//      inDir.open  ("doc1.txt", std::ifstream::in);
//    //inDir.open(tempFileName, std::ifstream::in);
//      do
//      {  inDir.getline(line, maxLineLength);
//         cout<<line<<"n";
//      }
//      while(!inDir.eof());
//      inDir.close();
//   }
//   catch(std::ifstream::failure e)
//   {  cout <<"Error opening fil "<< tempFileName <<" n";
//      cout << "Program Termites";
//      return -1;
//   }
//   return 0;
//   } // End of main()
   