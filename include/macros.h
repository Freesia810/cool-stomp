#ifndef __MACROS__
#define __MACROS__

#ifdef _WIN32
   #define __WINDOWS__
   #ifdef _WIN64
      #define __WIN_X64__
   #else
      #define __WIN_X86__
   #endif
#elif __APPLE__
    #if TARGET_IPHONE_SIMULATOR
        #define __IOS_SIMULATOR__
    #elif TARGET_OS_IPHONE
        #define __IOS__
    #elif TARGET_OS_MAC
        #define __MAC_OS__
    #endif
#elif __ANDROID__
    #define __ANDROID__
#elif __linux__
    #define __LINUX__
#elif __unix__
    #define __UNIX__
#elif defined(_POSIX_VERSION)
    #define __POSIX__
#endif

#ifdef __WINDOWS__
#define COOL_STOMP_PUBLIC   __declspec(dllexport)
#else
#define COOL_STOMP_PUBLIC 
#endif


#endif