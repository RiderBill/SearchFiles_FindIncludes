// SearchDirs8.h
#ifndef SEARCHDIRS
#define SEARCHDIRS

#include <TCHAR.h>
#include <string>
#include <vector>

using namespace std;

int findstringInFiless(vector<string>  searchString, 
                       vector<string>  parents     ,
                       vector<string>  files       ,
                       vector<string> &filesOut    ,
                       vector<string> &linesOut     );

vector<string> parseTokens(string line1, char *delimeters);

int search4Files(vector<string>          ParentDirs  ,
                 vector<vector<string>>  filesV      ,
                 vector<string>          extensions  ,
                 vector<string>         &foundParents,
                 vector<string>         &foundFiles   );

#endif // SEARCHDIRS