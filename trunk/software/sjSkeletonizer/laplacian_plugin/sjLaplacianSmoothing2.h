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
 * @date 21/01/2011 10:07 a.m.
 */

#ifndef __SJLAPLACE_SMOOTH__H__
#define __SJLAPLACE_SMOOTH__H__

#ifdef PLUGINLAPLACE_EXPORTS
  #define PLUGINLAPLACE_EXPORTS_API __declspec(dllexport)
#else
  #define PLUGINLAPLACE_EXPORTS_API __declspec(dllimport)
#endif

#include "sjKernelPlugin.h"

/*
class  sjLaplacianSmoothing: public sjMeshFilter{
public:

		PLUGINLAPLACE_EXPORTS_API virtual char * getName();
        PLUGINLAPLACE_EXPORTS_API virtual void setMesh(sjPolyhedron);
		PLUGINLAPLACE_EXPORTS_API virtual sjPolyhedron getMesh();
		PLUGINLAPLACE_EXPORTS_API virtual void configure();
		PLUGINLAPLACE_EXPORTS_API virtual void iterate();
		PLUGINLAPLACE_EXPORTS_API virtual void setParameters(sjParameterStore *);


	PLUGINLAPLACE_EXPORTS_API  sjLaplacianSmoothing();
	PLUGINLAPLACE_EXPORTS_API ~sjLaplacianSmoothing(){}
	void setMeshG(sjPolyhedron meshG);
	sjPolyhedron getMeshG();
	size_t getNsize();

	void initIndex();
	void computeRings();

	map<int, double> computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors);
	double averageFaces();
	double areaRing(sjVIterator vi, vector< sjVertex_handle> neighbors);
	bool isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors);

	sjPoint_3 computeMeanCurvature(sjVIterator vi, vector< sjVertex_handle> neighbors);

	void computeLineEquations(sjVIterator vi, vector< sjVertex_handle> neighbors);
	void meanCurvatureSmoothing();
	void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0);
	void iterateLaplacianSmoothingOGF();
	void iterateLaplacianSmoothingOGF2();
	
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
};*/

#endif
