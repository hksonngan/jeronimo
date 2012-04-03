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
 * The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */
#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJLOG__H__
#define __SJLOG__H__

#include <string>
//#include <stdarg.h>
//#include <stdio.h>

namespace sj {
	typedef enum {
		SJ_OFF_LOG_LEVEL     = 60000,
		SJ_FATAL_LOG_LEVEL   = 50000,
		SJ_ERROR_LOG_LEVEL   = 40000,
		SJ_WARN_LOG_LEVEL    = 30000,
		SJ_INFO_LOG_LEVEL    = 20000,
		SJ_DEBUG_LOG_LEVEL   = 10000,
		SJ_TRACE_LOG_LEVEL   = 0,
		SJ_ALL_LOG_LEVEL     = 0,
		SJ_NOT_SET_LOG_LEVEL = -1
	} SJ_LOG_LEVEL;
	class sjConfigLog{
	private:
		static sjConfigLog _instance;
		sjConfigLog();
	};

	 void sjLogDebug(const char* format, ...);
	 void sjLogDebug(std::string message);
	 void sjLogInformation(const char* format, ...);
	 void sjLogInformation(std::string message);
	 void sjLogWarning(const char* format, ...);
	 void sjLogWarning(std::string message);
	 void sjLogError(const char* format, ...);
	 void sjLogError(std::string message);
	 void sjLogSetLevel(SJ_LOG_LEVEL a_level);
	 void sjLogSetFileName(std::string);
	 void sjLogSetInstance(std::string);
}

#endif //__SJLOG__H__