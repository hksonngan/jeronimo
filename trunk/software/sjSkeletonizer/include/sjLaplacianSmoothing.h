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
 * @file sjLaplacianSmoothing.h
 * @brief sjSkeletonizer San Jeronimo Viewer Application.
 * @author Alexander Pinzon Fernandez.
 * @date 21/01/2011 10:07 a.m.
 */

#ifndef __SJLAPLACE_SMOOTH__H__
#define __SJLAPLACE_SMOOTH__H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>  
#include <map>

#include "sjDataType.h"
using namespace std;

class sjLaplacianSmoothing{
public:
	sjLaplacianSmoothing();
	void setMeshG(sjPolyhedron meshG);
	sjPolyhedron getMeshG();
	size_t getNsize();

	void initIndex();
	void computeRings();

	map<int, double> computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors);
	double averageFaces();
	double areaRing(sjVIterator vi, vector< sjVertex_handle> neighbors);
	bool isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors);

	void pruebaOpenNL();
	void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0);
	void iterateLaplacianSmoothing();
	void iterateLaplacianSmoothingOGF();
	
private:
	sjPolyhedron mesh_G;
	vector< vector<sjVertex_handle> > rings;
	double WH_0 ;
	double WL_0 ;
	double SL ;
	double AVERAGE_FACE;
	double MAX_CONTRACTION_SQUARED;	// Maximum contraction weight^2 before using hard constraint (default 100000)
	double MAX_ATTRACTION;		// Maximum attraction weight before using hard constraint    (default 1000)
	double MIN_COT_ANGLE;	// Minimum angle for cotan weights before clamping           (default 0.0000001)
	int iteration;
};

#endif
