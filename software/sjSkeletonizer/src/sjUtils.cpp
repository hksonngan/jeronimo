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

#include "sjUtils.h"

using namespace std;

double angle3(sjPoint_3 p1, sjPoint_3 p2, sjPoint_3 p3){
	sjVector_3 v1 = p1-p2;
	v1 = v1 / sqrt( v1 * v1);
	sjVector_3 v2 = p3-p2;
	v2 = v2 / sqrt( v2 * v2);
	//double anglex = std::acos( v1 * v2 ) / CGAL_PI * 180; 
	double anglex = std::acos( v1 * v2 ) ; 
	return anglex;
}

double area3(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c){
	sjVector_3 ab = b-a;
	sjVector_3 ac = c-a;
	sjVector_3 crossTriangle = CGAL::cross_product(ab, ac);
	double area = std::sqrt(crossTriangle*crossTriangle);
	return area/2.0;
}

void printVector(std::vector<double> x){
	int i, n = (int)(x.size());
	for(i=0; i<n; i++){
		cout<<x[i]<<" ";
	}
}

void printMatrix(std::vector<std::vector<double>> y){
	int i, n = (int)(y.size());
	int j, m = (int)(y[0].size());
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			cout<<y[i][j]<<" ";
		}
		cout<<"\n";
	}
}
