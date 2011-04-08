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

/** 
 * @brief sjSkeletonizer San Jeronimo Viewer Application.
 * @author Alexander Pinzon Fernandez.
 * @date 21/01/2011 10:07 a.m.
 */
#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJPLUGINLOADER__H__
#define __SJPLUGINLOADER__H__

#include <string>
#include "sjException.h"
#include "sjEngine.h"

namespace sj{
class sjPluginLoader
{
public:
	sjPluginLoader(void);
	~sjPluginLoader(void);
	SJHANDLER loadLibrary(std::string file_name) throw(sjException);
	void * getSymbol(SJHANDLER handle, std::string symbol_name) throw(sjException);
	void unloadLibrary(SJHANDLER handler) throw(sjException);
	std::string getError(SJHANDLER handler);
};
}//namespace SJ{
#endif //__SJPLUGINLOADER__H__