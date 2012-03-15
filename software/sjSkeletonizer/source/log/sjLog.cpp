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
#include <iomanip>

using namespace log4cplus;


void sj::sjLogDebug(std::string message){
	Logger logger = Logger::getInstance("mylogger");
	LOG4CPLUS_DEBUG(logger, message);
}

void sj::sjLogInformation(std::string message){
	//BasicConfigurator config;
	//config.configure();
	Logger logger = Logger::getInstance("mylogger");
	//LOG4CPLUS_INFO(logger, message);
	LOG4CPLUS_INFO(logger, message);
}

void sj::sjLogWarning(std::string message){
	//Logger logger = Logger::getInstance("main");
	//LOG4CPLUS_WARN(logger, message);
}

void sj::sjLogError(std::string message){
	Logger logger = Logger::getInstance("mylogger");
	LOG4CPLUS_ERROR(logger, message);
}

void sj::sjLogSetLevel(SJ_LOG_LEVEL a_level){
	//Logger logger = Logger::getInstance("main");
	//logger.setLogLevel(a_level);
}

void sj::sjLogSetFileName(std::string file_name){
}

void sj::sjLogSetInstance(std::string name_instance){
}