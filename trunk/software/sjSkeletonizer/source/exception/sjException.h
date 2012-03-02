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
	#pragma warning(disable: 4290)
#endif

#ifndef __SJEXCEPTION__H__
#define __SJEXCEPTION__H__

#include <iostream>
#include <string>
#include <stdexcept>

namespace sj{
	/**
	* @brief Base class for handling exceptions
	*/
	class sjException: public std::exception
	{
	public:
		typedef enum{
			SJ_EXCEPTION_DESTROY_ERROR,
			SJ_EXCEPTION_EMPTY_STRING,
			SJ_EXCEPTION_EXISTING_ELEMENT,
			SJ_EXCEPTION_LOAD,
			SJ_EXCEPTION_LOAD_SHARED_LIBRARY_FAILS,
			SJ_EXCEPTION_LOAD_SHARED_LIBRARY_SYMBOL_FAILS,
			SJ_EXCEPTION_IO,
			SJ_EXCEPTION_NOT_EXISTING_ELEMENT,
			SJ_EXCEPTION_NULL,
			SJ_EXCEPTION_UNKNOW
		} TypeException;

		sjException();
		sjException(std::string);
		sjException(TypeException);
		sjException(TypeException, std::string);
		~sjException();
		sjException(const sjException &Other);

		sjException::TypeException getType(void);
		std::string getMessage(void);

		void setType(sjException::TypeException);
		void setMessage(std::string);

	private:
		std::string message;
		TypeException type;
	};
}

#endif //__SJEXCEPTION__H__