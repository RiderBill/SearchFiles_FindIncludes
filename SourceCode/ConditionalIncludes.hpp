// ConditionalIncludes.hpp

#ifndef CONDTIONAL_INCLUDES
   #define CONDTIONAL_INCLUDES

   #define USERDATABILL
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

#endif // CONDTIONAL_INCLUDES
