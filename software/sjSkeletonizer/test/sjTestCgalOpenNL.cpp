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

//#include "sjLaplacianSmoothing.h"
#include <vector>
#include <cmath>  
#include <map>
#include <CGAL/Timer.h>
#include <fstream>
#include <iostream>

#include "sjDataType.h"
#include "sjUtils.h"
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 

#include <OGF/basic/types/counted.h>
#include <OGF/math/attributes/attribute_adapter.h>

#include <OGF/math/linear_algebra/vector.h>
#include <OGF/math/linear_algebra/matrix.h>
#include <OGF/math/geometry/types.h>
#include <OGF/math/numeric/linear_solver.h>

using namespace std;
using namespace OGF;
void main(){
	cout<<"\n";
	cout<<"Test Least Squares Solver with CGAL and OpenNL\n";
	sjLeastSquaresSolver solver(2);
	solver.set_least_squares(true) ;
	solver.begin_system() ;
	    solver.begin_row() ;
		solver.add_coefficient(0, 0.0)  ;
		solver.add_coefficient(1, 1.0)  ;
		solver.set_right_hand_side(1.0);
		solver.end_row() ;
		solver.begin_row() ;
		solver.add_coefficient(0, 1.0)  ;
		solver.add_coefficient(1, 0.0)  ;
		solver.set_right_hand_side(2.0);
		solver.end_row() ;
		solver.begin_row() ;
		solver.add_coefficient(0, 0.0)  ;
		solver.add_coefficient(1, 1.0)  ;
		solver.set_right_hand_side(3.0);
		solver.end_row() ;
	solver.end_system() ;
	if ( solver.solve() ){
		cout<<"\n\nResolvio el sistema.";
		cout<<solver.variable(0).value();
		cout<<"\n";
		cout<<solver.variable(1).value();
	}
}