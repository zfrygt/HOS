#ifndef MACROS_H
#define MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef COMMON_EXPORTS
		#define COMMON_EXPORT __declspec(dllexport)
	#else
		#define COMMON_EXPORT __declspec(dllimport)
	#endif
#else
	#ifdef COMMON_EXPORTS
		#define COMMON_EXPORT
	#endif
#endif

#endif
