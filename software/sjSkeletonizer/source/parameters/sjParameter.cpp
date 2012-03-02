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
#include "sjParameter.h"
#include <stdlib.h>


using namespace sj;

sjInfo::sjInfo(){
}
sjInfo::sjInfo(const sjInfo &Other){
	this->name = Other.name;
	this->description = Other.description;
}
sjInfo::~sjInfo(){
}
void sjInfo::setName(std::string a_name){
	this->name = a_name;
}
void sjInfo::setDescription(std::string a_description){
	this->description = a_description;
}
std::string sjInfo::getName(void){
	return name;
}
std::string sjInfo::getDescription(void){
	return description;
}

void * sjStringParam::getData(){
	std::string * str_pointer;
	*str_pointer = data;
	return str_pointer;
}

void sjStringParam::setData(void * a_data){
	std::string * dataT = (std::string *)a_data;
	data = *dataT;
}
		
sjTypeData sjStringParam::getType(){
	return SJ_DATA_TYPE_STRING;
}
		
void sjStringParam::setType(sjTypeData a_type){
}
		
std::string sjStringParam::getValue(){
	return data;
}

void sjStringParam::setValue(std::string a_data){
	data = a_data;
}


void * sjBoolParam::getData(){
	bool * bptr = new bool;
	*bptr = data;
	return (void *)bptr;
}
void sjBoolParam::setData(void * a_data){
	bool * dataT = (bool *)a_data;
	data = *dataT;
}
sjTypeData sjBoolParam::getType(){
	return SJ_DATA_TYPE_BOOLEAN;
}
void sjBoolParam::setType(sjTypeData a_type){
}
bool sjBoolParam::getValue(){
	return data;
}
void sjBoolParam::setValue(bool a_data){
	data = a_data;
}

void * sjTimeParam::getData(){
	time_t * dataptr = new time_t();
	* dataptr = data;
	return (void *)dataptr;
}
void sjTimeParam::setData(void * a_data){
	time_t * dataptr = (time_t *) a_data;
	data = *dataptr;
}
sjTypeData sjTimeParam::getType(){
	return SJ_DATA_TYPE_TIME;
}
void sjTimeParam::setType(sjTypeData a_type){
}
time_t sjTimeParam::getValue(){
	return data;
}
void sjTimeParam::setValue(time_t a_data){
	data = a_data;
}

/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////

sjParameterStore::sjParameterStore(void)
{
}

sjParameterStore::~sjParameterStore(void)
{
}

void sjParameterStore::addParameter(sjParameterBase * a_param){
	parameters.insert(std::map<std::string, sjParameterBase *>::value_type(a_param->getName(), a_param));
}

sjParameterBase * sjParameterStore::getParameter(std::string key)
{
	sjParameterBase * m_value2 = NULL;
	if(parameters.size() >0)
		if(parameters.find(key) != parameters.end()){
			sjParameterBase * m_value = (parameters.find(key))->second;
			return  m_value;
		}
	return m_value2;
}

sjParameterBase * sjParameterStore::getParameter(unsigned int index)
{
	sjParameterBase * m_value2 = NULL;
	if(parameters.size() >0)	{
		std::map<std::string, sjParameterBase *>::iterator i = parameters.begin();
		unsigned int n = 0;
		while(n<index && i != parameters.end()){
			n++;
			i++;
		}
		if(n==index && i != parameters.end()){
			sjParameterBase * m_value = i->second;
			return m_value;
		}
	}
	return m_value2;
}

unsigned int sjParameterStore::getNumberOfParameters(void){
	return (unsigned int)(parameters.size());
}