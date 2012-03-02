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

#ifndef __SJXML__H__
#define __SJXML__H__

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <string>

#include "sjException.h"

namespace sj{
	class sjXml{
	public:
		static bool validateDtd(std::string file_name_doc, std::string file_name_dtd) throw(sjException);
		static bool validateDocument(std::string file_name_doc) throw(sjException);
		static xmlNodeSetPtr getXmlNodeFromExpression(xmlDocPtr document, std::string expression);
		static int getSizeXmlNodeSet(xmlNodeSetPtr node);
		static std::string getStringFromNode(xmlDocPtr document, xmlNodeSetPtr node, int index);
		static std::string getStringFromExpression(xmlDocPtr document, std::string expression , int index = 0);
		static xmlDocPtr getXmlDocument(std::string file_name_doc);
		static xmlDocPtr parseXmlFile(std::string file_name_doc);
	};
}

#endif //__SJXML__H__