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
#include <CGAL/bounding_box.h>

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

double distance2Points(sjPoint_3 p1, sjPoint_3 p2){
	return std::sqrt((p1[0]-p2[0])*(p1[0]-p2[0])
		+ (p1[1]-p2[1])*(p1[1]-p2[1])
		+ (p1[2]-p2[2])*(p1[2]-p2[2]));
}

sjVector_3 normalVectorPoint3(sjPoint_3 p1, sjPoint_3 p2, sjPoint_3 p3){
	sjVector_3 U = p2-p1;
	sjVector_3 V = p3-p1;
	return CGAL::cross_product(U,V);

}	

sjVector_3 normalizeVector(sjVector_3 ve){
	return ve/std::sqrt(ve.squared_length () );
}


double area3(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c){
	sjVector_3 ab = b-a;
	sjVector_3 ac = c-a;
	sjVector_3 crossTriangle = CGAL::cross_product(ab, ac);
	double area = std::sqrt(crossTriangle*crossTriangle);
	return area/2.0;
}

/**
EFFICIENT FEATURE EXTRACTION FOR 2D/3D OBJECTS IN MESH REPRESENTATION
Cha Zhang and Tsuhan Chen
*/
double calcVolume(sjPolyhedron mesh_G){
	double vol = 0.0;
	int nface = 0;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3;

	for(sjFIterator f = mesh_G.facets_begin(); f !=mesh_G.facets_end(); ++f ){
		sjHalfedge_facet_circulator vcir = f->facet_begin();
		vector< sjVertex_handle> vertexs;
		do{
			sjVertex_handle punto = vcir->next()->vertex();
			vertexs.push_back(punto);
		}while(++vcir != f->facet_begin ());
		sjPoint_3 point_1 = vertexs[2]->point();
		sjPoint_3 point_2 = vertexs[1]->point();
		sjPoint_3 point_3 = vertexs[0]->point();
		x1 = point_1[0];
		y1 = point_1[1];
		z1 = point_1[2];

		x2 = point_2[0];
		y2 = point_2[1];
		z2 = point_2[2];

		x3 = point_3[0];
		y3 = point_3[1];
		z3 = point_3[2];

		vol = vol + (1.0/6.0)*(0.0-x3*y2*z1 + x2*y3*z1 + x3*y1*z2 - x1*y3*z2 - x2*y1*z3 + x1*y2*z3);
		nface++;

	}
	return vol;
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

double angleOrientedPlanes(sjHalfedge_handle hedge){
	sjVertex_handle vertex_A;
	sjVertex_handle vertex_B;
	sjVertex_handle vertex_C;
	sjVertex_handle vertex_D;

	vertex_A = hedge->vertex();
	vertex_B = hedge->next()->vertex();
	vertex_C = hedge->prev()->vertex();
	vertex_D = hedge->opposite()->next()->vertex();

	sjPoint_3 A,B,C,D;
	A = vertex_A->point();
	B = vertex_B->point();
	C = vertex_C->point();
	D = vertex_D->point();

	sjVector_3 vertex_CBA_normal = normalizeVector(normalVectorPoint3(C,B,A));
	sjVector_3 vertex_DCA_normal = normalizeVector(normalVectorPoint3(D,C,A));
	double lenBD = distance2Points(B,D);
	sjPoint_3 B_ext,D_ext;
	B_ext = B + (vertex_CBA_normal/500.0);
	D_ext = D + (vertex_DCA_normal/500.0);

	sjPoint_3 p1, p2, p3;
	p2 = sjPoint_3(0.0,0.0,0.0);
	p1 = p2 + vertex_CBA_normal;
	p3 = p2 - vertex_DCA_normal;

	double angle = angle3(p1, p2, p3);

	if(distance2Points(B_ext,D_ext) >=distance2Points(B,D)){
		angle = 2*sjpi - angle;
	}
	return angle;
}

sjIso_cuboid_3 calcBoundingBox(sjPolyhedron mesh){
	return CGAL::bounding_box(mesh.points_begin (), mesh.points_end ());
}

sjVector_3 normalVectorFromNeighbors(sjVIterator vi, vector< sjVertex_handle> neighbors){
	int i, n;
	n = neighbors.size();
	sjPoint_3 vnormal = sjPoint_3(0.0, 0.0, 0.0);
	for(i = 0; i<n; i++){
		sjVector_3 normali = normalizeVector(normalVectorPoint3(vi->point(), neighbors[i]->point(), neighbors[(i+1)%n]->point()));
		vnormal = vnormal + normalizeVector(normali.direction().vector());
	}
	vnormal = sjPoint_3(vnormal[0]/(double(n)), vnormal[1]/(double(n)), vnormal[2]/(double(n)));

	sjVector_3 vec = vnormal - sjPoint_3(0.0, 0.0, 0.0);
	vec = normalizeVector(vec);
	return vec;
}

double areaRing(sjVIterator vi, vector< sjVertex_handle> neighbors){
	int i, j, m, pos_1, pos_2;
	double area = 0.0;
	m = neighbors.size();
	for(j=0; j<m; j++){
		pos_1 = j;
		pos_2 = (j+1) % m;
		sjPoint_3 point_i = vi->point();
		area = area + area3(point_i,((neighbors[pos_1]))->point(),((neighbors))[pos_2]->point());
	}
	return area;
}

double averageFaces(sjPolyhedron mesh_G){
	double average = 0.0, area;
	int nface = 0;

	for(sjFIterator f = mesh_G.facets_begin(); f !=mesh_G.facets_end(); ++f ){
		sjHalfedge_facet_circulator vcir = f->facet_begin();
		vector< sjVertex_handle> vertexs;
		do{
			sjVertex_handle punto = vcir->next()->vertex();
			vertexs.push_back(punto);
		}while(++vcir != f->facet_begin ());
		area = area3(vertexs[0]->point(),vertexs[1]->point(),vertexs[2]->point());
		average = average + area;
		nface = nface + 1;
	}
	return average/((double)nface);
}