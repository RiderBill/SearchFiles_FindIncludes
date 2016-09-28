// fcn_s_MACROS.h

// These macros provide compatibility of the source code for older MSVC 
// versions and non-MSVC c++ compilers for the security enhanced funcions
// fscanf_s, sscanf_s, printf_s, strcpy_s, and fopen_s. 
// Surely other functions will be added in the future.
// Bill Brinson
// 21 February 2011

//#pragma once   // <-- I'm not sure whether it's better to do this with
                 //     a #pragma once statement or the #ifndef method.
#if !defined(__fcn_s_MACROS_h)
   #define   __fcn_s_MACROS_h

#include <cstdlib>
#include <cstdio>
#include <string>

   #if (!defined(_MSC_VER) || (_MSC_VER < 1600) )
      #define fscanf_s                       fscanf
      #define sscanf_s                       sscanf
      #define scanf_s                        scanf
      #define printf_s                       printf
      #define sprintf_s                      snprintf
      #define fprintf_s                      fprintf
      #define strcpy_s(s1, n1, s2)           strncpy    ( (s1), (s2), (n1) )
      #define fopen_s(fp, s1, s2)            *(fp)=fopen( (s1), (s2)       )
      #define _fcloseall                     fcloseall
      #define strtok_s                       strtok
      #define _strcmpi                       strcmpi
   #else
      #define fscanf                            fscanf_s                       
      #define sscanf                            sscanf_s                       
      #define scanf                             scanf_s                        
      #define printf                            printf_s
	 // The macro for sprintf to sprintf_s could fail at runtime if sizeof(dest) does not
      // evaluate to the max size of dest, e.g. if dest is not defined as char[const] dest;
      #define sprintf    ( (dest), (fmt)    )   sprintf_s(dest, sizeof(dest)-1), fmt,...)  

      #define fprintf                           fprintf_s                      
      #define strncpy    ( (s1), (s2), (n1) )   strcpy_s(s1, n1, s2)           
      #define (fp)=fopen( (s1), (s2)       )    fopen_s(fp, s1, s2)            
      #define fcloseall                         _fcloseall                     
      #define strtok                            strtok_s
      #define strcmpi                          _strcmpi
   #endif //_MSC_VER
#endif // __fcn_s_MACROS_h
