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
#include "sjXml.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

using namespace sj;

bool sjXml::validateDtd(std::string file_name_doc, std::string file_name_dtd) throw(sjException){
	int valid = 0;
	xmlValidCtxtPtr context;
	if((context = xmlNewValidCtxt()) == NULL){
		throw sjException(sjException::SJ_EXCEPTION_UNKNOW);
	}
	context->userData = (void *) stderr;
	context->error    = (xmlValidityErrorFunc) fprintf;
	context->warning  = (xmlValidityWarningFunc) fprintf;

	xmlDocPtr m_doc = NULL;
	m_doc = xmlReadFile(file_name_doc.c_str(), NULL, 0);
	xmlDtdPtr m_dtd = xmlParseDTD(NULL, (const xmlChar *)(file_name_dtd.c_str()));

	if(m_doc != NULL && m_dtd != NULL){
		valid = xmlValidateDtd(context, m_doc, m_dtd);
	}else{
		throw sjException(sjException::SJ_EXCEPTION_IO);
	}
	return (valid == 1);
}

bool sjXml::validateDocument(std::string file_name_doc) throw(sjException){
	int valid = 0;
	xmlValidCtxtPtr context;
	if((context = xmlNewValidCtxt()) == NULL){
		throw sjException(sjException::SJ_EXCEPTION_UNKNOW);
	}
	context->userData = (void *) stderr;
	context->error    = (xmlValidityErrorFunc) fprintf;
	context->warning  = (xmlValidityWarningFunc) fprintf;

	xmlDocPtr m_doc = NULL;
	m_doc = xmlReadFile(file_name_doc.c_str(), NULL, 0);

	if(m_doc != NULL ){
		valid = xmlValidateDocument(context, m_doc);
	}else{
		throw sjException(sjException::SJ_EXCEPTION_IO);
	}
	return (valid == 1);
}

xmlNodeSetPtr sjXml::getXmlNodeFromExpression(xmlDoc * document, std::string expression){
	xmlNodeSetPtr nodeset = NULL;
	xmlXPathContextPtr xpathCtx; 
	xmlXPathObjectPtr xpathObj; 

	xmlChar * xpath = (xmlChar *)(expression.c_str());

	xpathCtx = xmlXPathNewContext(document);
	if(xpathCtx == NULL){
		throw sjException(sjException::SJ_EXCEPTION_NULL, "Error in xmlXPathNewContext\n");
		return NULL;
	}

	xpathObj = xmlXPathEvalExpression(xpath, xpathCtx);
	xmlXPathFreeContext(xpathCtx);
	if(xpathObj == NULL){
		throw sjException(sjException::SJ_EXCEPTION_NULL, "Error in xmlXPathEvalExpression\n");
		return NULL;
	}

	if(xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)){
		xmlXPathFreeObject(xpathObj);
		return NULL;
	}

	nodeset = xpathObj->nodesetval;

	return nodeset;
}

int sjXml::getSizeXmlNodeSet(xmlNodeSetPtr node){
	if(node == NULL) return 0;
	return node->nodeNr;
}

std::string sjXml::getStringFromNode(xmlDocPtr document, xmlNodeSetPtr node, int index){
	xmlChar *keyword;
	if(document==NULL || node == NULL || index >= sjXml::getSizeXmlNodeSet(node)) return std::string("");
	keyword = xmlNodeListGetString(document, node->nodeTab[0]->xmlChildrenNode, 1);
	return std::string( reinterpret_cast<const char *>(keyword) );
}

std::string sjXml::getStringFromExpression(xmlDocPtr document, std::string expression , int index){
	xmlNodeSetPtr node = sjXml::getXmlNodeFromExpression(document, expression);
	return sjXml::getStringFromNode(document, node, index);
}

xmlDoc * sjXml::getXmlDocument(std::string file_name_doc){
	xmlDocPtr m_doc = NULL;
	m_doc = xmlReadFile(file_name_doc.c_str(), NULL, 0);
	return m_doc;
}

xmlDocPtr sjXml::parseXmlFile(std::string file_name_doc){
	xmlDocPtr m_doc = NULL;
	m_doc = xmlParseFile(file_name_doc.c_str());
	return m_doc;
}