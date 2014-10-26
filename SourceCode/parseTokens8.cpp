#include <string>
#include "sortedVector10.hpp"
#include "ConditionalIncludes.hpp"

using namespace std;
using namespace wcb;

vector<string> parseTokens(string line1, char *delimeters)
{  // First make a copy of the line so as to not obliterate the original.
   unsigned int sourceLength = line1.size();
   unsigned int destLength   = sourceLength + 1; // Leave room for terminating null
 //char *line = new (char) (destLength)  ;
   char *line = new char [destLength]  ;
   char *cptr, *nextTok = NULL;
   vector<string> tokens(0);

   strncpy_s(line, destLength, line1.c_str(), sourceLength);

   cptr = strtok_s(line, delimeters, &nextTok);
   while (cptr != NULL)
   {  tokens.push_back(string(cptr)); // Don't want these sorted nor unique.
      cptr = strtok_s(NULL, delimeters, &nextTok);
   }
   delete[] (line);
   return tokens;
} // End of parseTokens()


