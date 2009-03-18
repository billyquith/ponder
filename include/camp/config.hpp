// <insert copyright>

#ifndef CAMP_CONFIG_HPP
#define CAMP_CONFIG_HPP


// We define the CAMP_API macro according to the
// current operating system and build mode
#if defined(_WIN32) || defined(__WIN32__)

    // Windows platforms need specific keywords for import / export
    #ifdef CAMP_EXPORTS

        // From DLL side, we must export
        #define CAMP_API __declspec(dllexport)

    #else

        // From client application side, we must import
        #define CAMP_API __declspec(dllimport)

    #endif

#else

    // Other platforms don't need to define anything
    #define CAMP_API

#endif


#endif // CAMP_CONFIG_HPP
