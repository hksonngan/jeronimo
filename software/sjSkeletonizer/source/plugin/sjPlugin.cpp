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
#include "sjPlugin.h"
#include "sjException.h"
#include "sjPluginLoader.h"
using namespace sj;

sjPlugin::sjPlugin(const std::string & file_name)
:m_hDLL(0), m_pfnRegisterPlugin(0)
{
	sjPluginLoader loader;
	m_hDLL = loader.loadLibrary(file_name);
	if(!m_hDLL)
		throw sjException(sjException::SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS, loader.getError(m_hDLL));
	try{
		m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>(
			loader.getSymbol(m_hDLL, "registerPlugin"));
	}
	catch(...){
		loader.unloadLibrary(m_hDLL);
		throw;
	}
}

sjPlugin::sjPlugin(const sjPlugin &Other):
m_hDLL(Other.m_hDLL),
m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin)
{}

sjPlugin::~sjPlugin(void){
	sjPluginLoader loader;
	loader.unloadLibrary(m_hDLL);
}