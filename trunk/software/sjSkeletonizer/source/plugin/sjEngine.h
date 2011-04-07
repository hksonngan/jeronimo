///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef __SJENGINE__H__
#define __SJENGINE__H__

#if defined (WIN32) || defined (_WIN32_)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#define SJHANDLER HMODULE
#else
#ifdef __APPLE__
	#include <mach-o/dyld.h>
	#include <dlfcn.h>
	#include <stdarg.h>
	#define SJHANDLER void * 
#else //UNIX
	#include <dlfcn.h>
	#define SJHANDLER void * 
#endif //__APPLE__
#endif //WIN32

#if defined (WIN32) || defined (_WIN32_)
	#ifdef SJENGINE_EXPORTS
		#define SJENGINE_API __declspec(dllexport)
	#else
		#define SJENGINE_API __declspec(dllimport)
	#endif
#else
	#define SJENGINE_API
#endif

#endif // __SJENGINE__H__