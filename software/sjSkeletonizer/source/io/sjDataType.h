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

#ifndef __SJDATATYPE__H__
#define __SJDATATYPE__H__

#include <CGAL/Simple_cartesian.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Polyhedron_3.h>
#include <vector>
#include <CGAL/Taucs_solver_traits.h>
#include <CGAL/OpenNL/linear_solver.h>
#include <OGF/math/linear_algebra/vector.h>
#include <OGF/math/linear_algebra/matrix.h>
#include <iostream>

using namespace std;
using namespace OGF;

template <class Refs, class T, class P>
class sjVertexType : public CGAL::HalfedgeDS_vertex_base<Refs,T, P>{
public:
	sjVertexType() {}
	sjVertexType(const P& pt): CGAL::HalfedgeDS_vertex_base<Refs, T, P>(pt){}
	int index;
	double initial_ring_area;
	double a1,a2;
	double b1,b2;
	double c1,c2;
	double d1,d2;
};

template <class Refs>
class Halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
public:
   Halfedge() :
      hedgeid(-1){
   }
   int      hedgeid; // halfedge id
};

/*template <class Refs>
struct sjFaceType : public CGAL::HalfedgeDS_face_base<Refs> {
    int index;
	double area;
};*/

struct sj_ItemsType : public CGAL::Polyhedron_items_3 {
	template <class Refs, class Traits>
    struct Vertex_wrapper {
        typedef typename Traits::Point_3 Point;
		typedef sjVertexType<Refs, CGAL::Tag_true, Point> Vertex;
    };

	template <class Refs, class Traits>
   struct Halfedge_wrapper {
      typedef Halfedge<Refs>                             Halfedge;
   };

	/*template <class Refs, class Traits>
    struct Face_wrapper {
        typedef sjFaceType<Refs> Face;
    };*/
};

typedef CGAL::Simple_cartesian<double>					sjKernel;
typedef sjKernel::Point_3								sjPoint_3;
typedef sjKernel::Vector_3								sjVector_3;
typedef CGAL::Polyhedron_3<sjKernel, sj_ItemsType>		sjPolyhedron;
typedef sjKernel::Iso_cuboid_3							sjIso_cuboid_3;
typedef sjPolyhedron::Face_iterator						sjFIterator;
typedef sjPolyhedron::Halfedge_iterator					sjHEIterator;
typedef sjPolyhedron::Halfedge_handle					sjHalfedge_handle;
typedef sjPolyhedron::Vertex_iterator					sjVIterator;
typedef sjPolyhedron::Vertex_handle						sjVertex_handle;
typedef sjPolyhedron::Point_iterator					sjPoint_iterator;
typedef sjPolyhedron::Halfedge_around_facet_circulator	sjHalfedge_facet_circulator;
typedef sjPolyhedron::Halfedge_around_vertex_circulator	sjHalfedge_vertex_circulator;

//typedef CGAL::Taucs_solver_traits<sjKernel::FT> sjTaucsSolver;
//typedef sjTaucsSolver::Matrix sjTaucsMatrix;
//typedef sjTaucsSolver::Vector sjTaucsVector;
//typedef OpenNL::SymmetricLinearSolverTraits<sjKernel::FT> sjSparseLinearAlgebraTraits_d;
typedef OpenNL::DefaultLinearSolverTraits<sjKernel::FT> sjSparseLinearAlgebraTraits_d;

typedef OpenNL::LinearSolver<sjSparseLinearAlgebraTraits_d> sjLeastSquaresSolver ;
typedef sjSparseLinearAlgebraTraits_d::Matrix sjOpenNLMatrix;
typedef sjSparseLinearAlgebraTraits_d::Vector sjOpenNLVector;

typedef vector<double> sjVectorDouble;
typedef vector<vector<double>> sjMatrixDouble;

typedef Matrix<double, 4> Matrix4d;

#endif // __SJDATATYPE__H__