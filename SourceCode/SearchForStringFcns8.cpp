#include <stdlib.h>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, ofstream
#include <string>
#include <cstring>      // strerror()
#include <vector>
#include <limits>
#include "sortedVector10.hpp"
#include "ConditionalIncludes.hpp"
#include "SearchForStringFcns8.hpp"
#include "parseTokens8.hpp"

using namespace std;

int findStringInFiles(vector<string>  searchString, 
                      vector<string>  parents     ,
                      vector<string>  files       ,
                      vector<string> &parentsOut  ,
                      vector<string> &filesOut    ,
                      vector<string> &linesOut     )
{  ////////////////////////////////////////////////////////////////////////////
   // This function searches through files for the existence of string(s).
   // All lines containing the string(s) are saved to a string vector
   // Input:
   //   searchString - the string(s) to search for
   //   Parents      - the full or relative Parent directories of the files
   //                  including a trailing \ or /
   //   Files        - the filenames to search
   // Output:
   //   filesOut     - full or relative pathnames corresponding to the
   //                  lines found with matching text
   //   linesOut     - lines found with matching text
   //   
   // Notes:
   //   - There should be a one to one correspondence between
   //     parents and files.
   //   - Parent strings are expected to include a trailing \ or /
   //   - filesOut.size() and linesOut.size() will be the same.  I.e.
   //     each element of filesOut corresponds to a found line.
   //
   // Programmer   : Bill Brinson
   // Last Mod Date: 2014-09-30
   ////////////////////////////////////////////////////////////////////////////
   const size_t maxLineLength = 512;
   char line[maxLineLength+1];

   // Loop through filenames looking for seachString(s)
   int nFiles = files.size();
   for(int iFile=0; iFile<nFiles; iFile++)
   {  ifstream inFs;
      inFs.exceptions ( std::ifstream::failbit );
      try
      {  // I think the file needs to be open as a binary file for this to work right.
         inFs.open((parents[iFile]+files[iFile]).c_str(), std::ios_base::in
                                                        | std::ios_base::binary);
 if (files[iFile].compare("marc211w.cpp") == 0)
 {  printf("foundit\n");
 }

         // Loop through search strings
         for(vector<string>::iterator its = searchString.begin(); its<searchString.end(); its++)
         {  // Find the positions of all occurances of the currrent search string.
            size_t ifpos = 0;
            size_t maxToFind = 0; // 0 - all occurances.
            size_t nFound; 
            vector<size_t> foundPositions;
            searchForNStrings(inFs, its->c_str(), maxToFind, nFound, foundPositions );

            // Go back through file to get lines containing search string.
            for(size_t ifound=0; ifound<nFound; ifound++)
            {  // Move back to the beginning of the line.
               // First move inFs back to the start of the search string.
               inFs.seekg(foundPositions[ifound], ios_base::beg);

               // Now step back one char at a time until start of file or newline is found.
               while (  ( (size_t)inFs.tellg()  > 0 )
                      &&( inFs.get() != '\n'        ) )
               {  inFs.seekg(-2L, ios_base::cur); // One to put back the char just read,
               }                                  // and one to move back one more char.

               // Get the line starting with the search string.
               inFs.getline(line, maxLineLength);

               // Add parent directory, filename, and the line to output vectors.
               parentsOut.push_back(parents[iFile]);
               filesOut  .push_back(files  [iFile]);
               linesOut  .push_back(line          );
            } // End of for(ifound)

            //Rewind file for next search.
            inFs.seekg(0, ios_base::beg);

         } // End of for(its)
         inFs.close();
      } // End of try{}
      catch(std::ifstream::failure &e)
      {  std::cerr << "Exception opening/reading/closing file: " << endl;
         std::cerr << parents[iFile] << files[iFile] << endl;
         std::cerr << e.what() << endl;
         // Just continue to the next file.
      } // End of catch{}
   }
   return 0;
} // End of findstringsInFiles()


// searchForString overloaded functions
//******************************************************************************
//		The searchForString routines search an ifstream, FILE* or string object
// for the next occurance of occurance of an ASCCI string. The starting
// for the search is at the current file position, or at the sPos'th character
// in the case of a string object search.
// If the string is found, the file pr string position at the
//	beginning of the searched for string is returned in fpos, measured in
// characters from the beginning of the file or string.
// If the string is not found, the file position is set to its the 
// initial position and the file is rewound to that point.
// Note: the file is presumed to be opened for text input by the calling
// funcion.
//
// Inputs:
//    ifs        - ifstream or FILE pointer or string object to search through
// or fp          (files are presumed already opened for input).
// or targText 
//    searchText - string object to search for.
//
// outputs:      
//    position   - file or string position at start of found string.
//                 Original file position if string is not found in a file,
//                 strlen(targTex) + 1 for 
//                 Check return value (bool) to determine if string was found.
// Return Value:
//   true if search string found, false otherwise.
//
// Exceptions:
//   The ifstream exception mask is cleared to prevent an eof event from 
//   throwing an exception.  The exception mask is reset to its original
//    state upon return.
//
// Programmer   : Bill Brinson
// Last Mod Date: 2014-10-02
//******************************************************************************
bool searchForString  (ifstream     &ifs      , string const   &searchText, size_t &position)
{//char  cc; // ifstream extration operator returns a char (not an int).
 //size_t ii;
	size_t searchTextLength = searchText.size();
   streampos startPos = ifs.tellg(); // Save starting position for future use.
   const size_t searchStrLen = searchText.size();
   char * readString = new char [searchStrLen + 1];
   bool ieof;
   // Save exception mask and the format flags.
   std::ios::iostate excpMask = ifs.exceptions();
   std::ios::fmtflags formatFlags = ifs.flags();

   // Clear the exception mask and set noskipsws.
   ifs.exceptions(std::ifstream::goodbit);
   std::noskipws(ifs);

   do
   {  ifs.read(readString, searchStrLen);
      streamsize gc = ifs.gcount();
      if (gc < searchStrLen)
      {  break;  // Not enough characters left in ifs to match searchString.
      }
      if (strncmp(readString, searchText.c_str(), searchStrLen) == 0)
      {  // searchString found.
         ifs.exceptions(excpMask);
         ifs.flags(formatFlags);
         position = (size_t)ifs.tellg() - searchStrLen;
         ifs.seekg(position, ios_base::beg);
         delete [] readString;
         return true;
      }
      // If at EOF, string will not be found.
      if( (ieof = ifs.eof()) )
         break;

	   // If string was not found, reposition file pointer to the character
	   // following the character which matched the first character of
	   // string.
      position = (size_t) ifs.tellg();
      ifs.seekg( (position - (searchStrLen - 1)), ios_base::beg);
      position = (size_t) ifs.tellg();
	} while ( !ifs.eof());
   
	// String was not found.  Reset file pointer to original position
   // and Return false.
   delete [] readString;
   ifs.clear();
   ifs.exceptions(excpMask);
   ifs.flags(formatFlags);
   position = (size_t) startPos;
   ifs.seekg(position, ios_base::beg);
	return false;
} // End of searchForString(ifstream &ifs, string const &searchText, size_t &position)


bool searchForString  (FILE         *fp       , string const   &searchText, size_t &position)
{	int  cc; // fgetc returns type int, not char.
	size_t ii, searchTextLength = searchText.size();
   size_t startPos = ftell(fp); // Save original file position for future use.

	do
	{	/* Read through file checking for the string. */
		ii = 0;
		do
		{	if (ii == searchTextLength)
         {  // searchString found.
            position = ftell(fp) - ii;
            fseek(fp, position, SEEK_SET);
            return true;
         }
		}  while (  ( ( cc  = fgetc(fp) ) == (int) searchText[ii++] )
                &&(   cc != EOF                                   ) ) ;

      // If at EOF, string will not be found.
      if( cc == EOF ) break;

		/* If string was not found, reposition file pointer to the character	*/
		/* following the character which matched the first character of		*/
		/* string (unless the file is already positioned at that point).		*/
		if (ii > 1)  fseek( fp, ((long) (1 - ii)), SEEK_CUR );
	} while ( cc != EOF );

	/* String was not found.  Reset file pointer to original position
      and Return 0L.						*/
   fseek( fp, startPos, SEEK_SET );
	return false;
} // End of (FILE *fp       , string const &searchText, size_t &position)


bool searchForString  (string const &targText , string const   &searchText, size_t &position)
{	size_t targLength = targText.size();
	size_t ii, searchTextLength = searchText.size();
   size_t startPos = position; // Save starting position in targText.

	do
	{	// Read through targText checking for the searchText.
		ii = 0;
		do
		{	if (ii == searchTextLength)
         {  position -= ii;  // -= searchTextLength
            return true;
         }
		}  while (  (position < targLength                    )
                &&(targText[position++] == searchText[ii++] ) );

		// If string was not found, reposition targText index to the character
		// following the character which matched the first character of
		// SearchText (unless the string is already positioned at that point).
		if (position > 1)  position -= 1 - ii;
	} while ( position < targLength );

	// String was not found.  Return false.
   position = targLength;  // One past the end of the string.
                           // This is redundant since position must be >= targLength,
                           // i.e. == targLength, to get here.
	return false;
} // End of searchForString(string const &targText, string const &searchText, size_t &position)


bool searchForNStrings(ifstream     &ifs      , string const   &searchText    , 
                       size_t        maxToFind,
                       size_t       &nFound   , vector<size_t> &foundPositions )
{  bool success = false;
   size_t foundPos  ;

   nFound = 0;
   while(   (!ifs.eof()                                 )
         && ( (nFound < maxToFind) || (maxToFind == 0)  )
         && (searchForString(ifs, searchText, foundPos) ) )
   {  success = true;
      nFound++;
      foundPositions.push_back(foundPos);
      // move file one char forward before starting next search.
      ifs.seekg(1, ios_base::cur);
   }
   return success;
} // End of searchForNStrings( ifstream & const ifs, string const &searchText, ...)


bool searchForNStrings(FILE         *fp       , string const   &searchText    ,
                       size_t        maxToFind,
                       size_t       &nFound   , vector<size_t> &foundPositions )
{  bool success = false;
   size_t foundPos  ;

   nFound = 0;
   while(   ( (nFound < maxToFind) || (maxToFind == 0)  )
         && (searchForString(fp, searchText, foundPos) ) )
   {  success = true;
      nFound++;
      foundPositions.push_back(foundPos);
      // move file one char forward before starting next search.
      fseek(fp, 1, SEEK_CUR);
   }
   return success;
} // End of searchForNStrings( FILE * const fp, string const &searchText, ...)


bool searchForNStrings(string const &targText , string const   &searchText    ,
                       size_t        maxToFind, size_t          startPos      ,
                       size_t       &nFound   , vector<size_t> &foundPositions )
{  bool success = false;
   size_t position = startPos;

   nFound = 0;
   while(   ( (nFound < maxToFind) || (maxToFind == 0)  )
         && (searchForString(targText, searchText, position) ) )
   {  success = true;
      nFound++;
      foundPositions.push_back(position);
      // Start next search one character forward.
      position++;
   }
   return success;
} // End of searchForNStrings( string const &targText, string const &searchText, ...)
