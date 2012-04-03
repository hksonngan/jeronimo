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
#include "sjLog.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
//#include <iomanip>
#include <exception>
using namespace log4cplus;

sj::sjConfigLog sj::sjConfigLog::_instance;

sj::sjConfigLog::sjConfigLog(){
	try
	{
	PropertyConfigurator::doConfigure("logs.properties");
	}
	catch(std::exception e)
	{

		printf("Exception occured while opening logs.properties");
		printf(e.what());
	}
}


inline void sj::sjLogDebug(const char* format, ...){
	va_list args;
	char buffer[BUFSIZ];
	va_start(args,format);
	sprintf (buffer, format, args );
	va_end(args);
	Logger logger = Logger::getInstance("debuglogger");
	LOG4CPLUS_DEBUG(logger, buffer);
}
inline void sj::sjLogDebug(std::string message){
	Logger logger = Logger::getInstance("debuglogger");
	LOG4CPLUS_DEBUG(logger, message);
}

inline void sj::sjLogInformation(const char* format, ...){
	va_list args;
	char buffer[BUFSIZ];
	va_start(args,format);
	sprintf (buffer, format, args );
	va_end(args);
	Logger logger = Logger::getInstance("infologger");
	LOG4CPLUS_INFO(logger, buffer);
}
inline void sj::sjLogInformation(std::string message){
	Logger logger = Logger::getInstance("infologger");
	LOG4CPLUS_INFO(logger, message);
}

inline void sj::sjLogWarning(const char* format, ...){
	va_list args;
	char buffer[BUFSIZ];
	va_start(args,format);
	sprintf (buffer, format, args );
	va_end(args);
	Logger logger = Logger::getInstance("warninglogger");
	LOG4CPLUS_WARN(logger, buffer);
}
inline void sj::sjLogWarning(std::string message){
	Logger logger = Logger::getInstance("warninglogger");
	LOG4CPLUS_WARN(logger, message);
}

inline void sj::sjLogError(const char* format, ...){
	va_list args;
	char buffer[BUFSIZ];
	va_start(args,format);
	sprintf (buffer, format, args );
	va_end(args);
	Logger logger = Logger::getInstance("errorlogger");
	LOG4CPLUS_ERROR(logger, buffer);
}
inline void sj::sjLogError(std::string message){
	Logger logger = Logger::getInstance("errorlogger");
	LOG4CPLUS_ERROR(logger, message);
}

inline void sj::sjLogSetLevel(SJ_LOG_LEVEL a_level){
	//Logger logger = Logger::getInstance("main");
	//logger.setLogLevel(a_level);
}

inline void sj::sjLogSetFileName(std::string file_name){
}

inline void sj::sjLogSetInstance(std::string name_instance){
}