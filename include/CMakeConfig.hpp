#ifndef CMAKECONFIG_HPP
#define CMAKECONFIG_HPP

    #ifdef ROOT_PATH_CMAKE
    #define ROOT_PATH QT_STRINGIFY(ROOT_PATH_CMAKE)
    #else
    #define ROOT_PATH ""
    #endif
    
#endif // CMAKECONFIG_HPP
