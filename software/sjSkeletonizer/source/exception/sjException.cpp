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
#include "sjException.h"
#include "sjLog.h"
using namespace sj;

sjException::sjException(){
	type = SJ_EXCEPTION_UNKNOW;
	message = "";
}

sjException::sjException(std::string a_msg){
	message = a_msg;
	type = SJ_EXCEPTION_UNKNOW;
	sjLogError(std::string("sjException: ") + a_msg);
}

sjException::sjException(TypeException a_type){
	type = a_type;
	message = "";
	switch(a_type){
		case SJ_EXCEPTION_DESTROY_ERROR: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_DESTROY_ERROR")); break;
		case SJ_EXCEPTION_EMPTY_STRING: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_EMPTY_STRING")); break;
		case SJ_EXCEPTION_EXISTING_ELEMENT: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_EXISTING_ELEMENT")); break;
		case SJ_EXCEPTION_LOAD: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_LOAD")); break;
		case SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS")); break;
		case SJ_EXCEPTION_LOAD_SHARED_LIBRARY_SYMBOL_FAILS: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_LOAD_SHARED_LIBRARY_SYMBOL_FAILS")); break;
		case SJ_EXCEPTION_IO: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_IO")); break;
		case SJ_EXCEPTION_NOT_EXISTING_ELEMENT: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_NOT_EXISTING_ELEMENT")); break;
		case SJ_EXCEPTION_NULL: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_NULL")); break;
		case SJ_EXCEPTION_UNKNOW: sjLogError(std::string("sjException: ") + std::string("SJ_EXCEPTION_UNKNOW")); break;
		default: sjLogError(std::string("sjException: unknow")); 
	}
}

sjException::sjException(TypeException a_type, std::string a_msg){
	type = a_type;
	message = a_msg;
	sjLogError(std::string("sjException: ") + a_msg);
}

sjException::TypeException sjException::getType(void){
	return type;
}
std::string sjException::getMessage(void){
	return message;
}

void sjException::setType(sjException::TypeException a_type){
	type = a_type;
}
void sjException::setMessage(std::string a_msg){
	message = a_msg;
}

sjException::~sjException(){
}

sjException::sjException(const sjException &Other){

}
