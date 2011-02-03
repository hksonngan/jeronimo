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

#include "sjDataType.h"
using namespace std;

class sjLaplacianSmoothing{
public:
	sjLaplacianSmoothing();
	void setMeshG(sjPolyhedron meshG);
	void setMatrixL(sjMatrixDouble matrixL);
	void setMatrixV(sjMatrixDouble matrixV);
	void setMatrixVprima(sjMatrixDouble matrixVprima);
	void setMatrixWl(sjVectorDouble matrixWl);
	void setMatrixWh(sjVectorDouble matrixWh);
	void setRings(vector<vector< sjVertex_handle> > aRings);

	sjPolyhedron getMeshG();
	sjMatrixDouble getMatrixL();
	sjMatrixDouble getMatrixV();
	sjMatrixDouble getMatrixVprima();
	sjVectorDouble getMatrixWl();
	sjVectorDouble getMatrixWh();
	vector<vector< sjVertex_handle> > getRings();
	size_t getNsize();

	sjMatrixDouble computeL();
	void initVertexIndex();
	sjVectorDouble initRatioMatrixWl(double ratio = 0.001);
	sjVectorDouble initRatioMatrixWh(double ratio = 1.0);
	
	sjVectorDouble updateMatrixWh(sjVectorDouble WH);
	sjVectorDouble updateMatrixWl(sjVectorDouble WL);
	sjMatrixDouble multiplyWlxL(sjVectorDouble WL, sjMatrixDouble L);
	sjVectorDouble multiplyWhxV(sjVectorDouble WH, sjMatrixDouble V, int coord);

	void findRings();
	double averageFace();
	sjVectorDouble areaRings();

	void pruebaOpenNL();
	void initLaplacianSystem();
	void iterateLaplacianSystem();
	
private:
	sjPolyhedron mesh_G;
	sjMatrixDouble matrix_L;
	sjMatrixDouble matrix_V;
	sjMatrixDouble matrix_Vprima;
	sjVectorDouble matrix_Wl;
	sjVectorDouble matrix_Wh;
	vector<vector< sjVertex_handle> > rings;
	sjVectorDouble area_rings;
	sjVectorDouble area_rings_original;
	double const_wh ;
	double const_SL ;
	int iteration;
};

#endif
