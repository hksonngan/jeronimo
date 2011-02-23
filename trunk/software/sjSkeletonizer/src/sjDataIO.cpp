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
 * @date 21/02/2011 5:27 p.m.
 */

#include "sjDataIO.h"
#include "sjDataType.h"

sjDataIO::sjDataIO(){
}

void sjDataIO::setFileName(string a_file_name){
	file_name = a_file_name;
}

string sjDataIO::getFileName(){
	return file_name;
}

void sjDataIO::load() throw(exception){
	if(file_name.empty()) throw exception("File Name is empty");
	if(file_name.compare("") == 0) throw exception("File Name is empty");
	
	ifstream file_off(file_name.c_str(), ios::in);
	if(!file_off.is_open()) throw exception("Not open File");

	file_off>>polyhedron_model;
	file_off.close();
}

sjPolyhedron sjDataIO::getPolyhedronModel(){
	return polyhedron_model;
}