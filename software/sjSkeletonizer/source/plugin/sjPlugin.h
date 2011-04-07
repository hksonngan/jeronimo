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

#ifndef __SJPLUGIN__H__
#define __SJPLUGIN__H__

#include "sjEngine.h"
#include <string>

namespace sj{

class sjKernelEngine;

class sjPlugin
{
public:
	SJENGINE_API sjPlugin(const std::string &);
	SJENGINE_API sjPlugin(const sjPlugin &Other);
	SJENGINE_API ~sjPlugin(void);
	
	SJENGINE_API void registerPlugin(sjKernelEngine & K) {
      m_pfnRegisterPlugin(K);
    }
private:
	sjPlugin &operator =(const sjPlugin &Other);
	typedef void fnRegisterPlugin(sjKernelEngine &);
	fnRegisterPlugin * m_pfnRegisterPlugin;
	SJHANDLER m_hDLL;
};
} //namespace SJ
#endif //__SJPLUGIN__H__