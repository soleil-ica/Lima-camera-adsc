#ifndef SIMUCOMPATIBILITY_H
#define SIMUCOMPATIBILITY_H

#ifdef WIN32
#ifdef LIBADSC_EXPORTS
#define LIBADSC_API __declspec(dllexport)
#else
#define LIBADSC_API __declspec(dllimport)
#endif
#else  /* Unix */
#define LIBADSC_API
#endif

#endif
