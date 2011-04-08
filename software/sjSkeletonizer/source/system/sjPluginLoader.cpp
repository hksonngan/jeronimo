/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Alexander Pinzon Fernandez.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */
#include "sjPluginLoader.h"
#include "sjLog.h"

#if defined (WIN32) || defined (_WIN32_)
#include <windows.h>
#else 
#include <dlfcn.h>
#endif

using namespace sj;

sjPluginLoader::sjPluginLoader(void){
	sjLogInformation(std::string("Subsystem sjPluginLoader initializing...\n"));
}

sjPluginLoader::~sjPluginLoader(void){
}

SJHANDLER sjPluginLoader::loadLibrary(std::string file_name) throw(sjException){
	sjLogDebug(std::string("Load shared library: ") + file_name);
#if defined (WIN32) || defined (_WIN32_)
	SJHANDLER handle = LoadLibrary(file_name.c_str());
#else
	SJHANDLER handle = dlopen(name, RTLD_LAZY);
#endif
	if(handle == NULL) throw sjException(sjException::SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS, getError(handle));
	return handle;
}

void * sjPluginLoader::getSymbol(SJHANDLER handle, std::string symbol_name) throw(sjException){
	sjLogDebug(std::string("Load symbol from library: ") + symbol_name);
	if(handle == NULL)  throw sjException(sjException::SJ_EXCEPTION_NULL);
#if defined (WIN32) || defined (_WIN32_)
	void * proc = GetProcAddress(handle, symbol_name.c_str());
#else
	void * proc = dlsym(handle, symbol_name.c_str());
#endif
	if(proc == NULL) throw sjException(sjException::SJ_EXCEPTION_LOAD_SHARED_LIBRARY_SYMBOL_FAILS, getError(handle));	 
	return proc;
}

void sjPluginLoader::unloadLibrary(SJHANDLER handler) throw(sjException){
	sjLogDebug(std::string("Unload shared library: "));
#if defined (WIN32) || defined (_WIN32_)
	FreeLibrary(handler);
#else
	dlclose(handler);
#endif
}

std::string sjPluginLoader::getError(SJHANDLER handler){
	std::string str_error = "";
#if defined (WIN32) || defined (_WIN32_)
	int err;
	err= GetLastError();
	if (!handler) SetLastError(ERROR_SUCCESS);
	if (err) {
		static char buf[1024];
		if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, 
					err, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					buf, 
					sizeof(buf), 
					NULL))
					str_error = std::string( buf);
	}
#else
	char *errstr;
	errstr = dlerror();
	if(errstr != NULL) str_error = std::string( errstr); 
#endif
	return str_error;
}