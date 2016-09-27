// ConditionalIncludes.hpp

#ifndef CONDTIONAL_INCLUDES
   #define CONDTIONAL_INCLUDES

   #if (defined USERDATABILL)
      #include <U:\\Bill\\LANG\\VCC_2012\\Brin_Inc\\fcn_s_MACROS.h>
      #if (defined(_MSC_VER))
         #include <U:\\Bill\\LANG\\dirent_include\\dirent.h>
      #else
        #include <dirent.h>
      #endif // if(defined(_MSC_VER)
   #else
      #include "fcn_s_MACROS_2016-01-19"
      #if (defined(_MSC_VER))
         #include "dirent.h"
      #else
//      I'm not sure either will work on a generic Linux box.
//      dirent.h is a boost file. I copied the version I have on my Windows
//      machine to the project sourcecode directory. I wouldn't think
//      a booste include file would care which OS it's compiled under.
//      #include <dirent.h>
        #include "dirent.h"
      #endif // if(defined(_MSC_VER)
   #endif // if/else (defined USERDATABILL)

#endif // CONDTIONAL_INCLUDES
