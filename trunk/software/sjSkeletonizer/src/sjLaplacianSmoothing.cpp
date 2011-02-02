#include "sjLaplacianSmoothing.h"
#include <vector>
#include <cmath>  

#include "sjDataType.h"
#include "sjUtils.h"
using namespace std;

sjLaplacianSmoothing::sjLaplacianSmoothing():const_wh(1.0), const_SL(2.0), iteration(0){
}

void sjLaplacianSmoothing::setMeshG(sjPolyhedron meshG){
	mesh_G = meshG;
}
void sjLaplacianSmoothing::setMatrixL(sjMatrixDouble matrixL){
	matrix_L = matrixL;
}
void sjLaplacianSmoothing::setMatrixV(sjMatrixDouble matrixV){
	matrix_V = matrixV;
}
void sjLaplacianSmoothing::setMatrixVprima(sjMatrixDouble matrixVprima){
	matrix_Vprima = matrixVprima;
}
void sjLaplacianSmoothing::setMatrixWl(sjVectorDouble matrixWl){
	matrix_Wl = matrixWl;
}
void sjLaplacianSmoothing::setMatrixWh(sjVectorDouble matrixWh){
	matrix_Wh = matrixWh;
}
void sjLaplacianSmoothing::setRings(vector<vector< sjVertex_handle> > aRings){
	rings = aRings;
}

sjPolyhedron sjLaplacianSmoothing::getMeshG(){
	return mesh_G;
}
sjMatrixDouble sjLaplacianSmoothing::getMatrixL(){
	return matrix_L;
}
sjMatrixDouble sjLaplacianSmoothing::getMatrixV(){
	return matrix_V;
}
sjMatrixDouble sjLaplacianSmoothing::getMatrixVprima(){
	return matrix_Vprima;
}
sjVectorDouble sjLaplacianSmoothing::getMatrixWl(){
	return matrix_Wl;
}
sjVectorDouble sjLaplacianSmoothing::getMatrixWh(){
	return matrix_Wh;
}
vector<vector< sjVertex_handle> > sjLaplacianSmoothing::getRings(){
	return rings;
}

size_t sjLaplacianSmoothing::getNsize(){
	return mesh_G.size_of_vertices();
}

void sjLaplacianSmoothing::initVertexIndex(){
	int i = 0;
	for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		sjVectorDouble vectemp;
		vectemp.push_back(vcir->vertex()->point().x());
		vectemp.push_back(vcir->vertex()->point().y());
		vectemp.push_back(vcir->vertex()->point().z());
		matrix_V.push_back(vectemp);
		i++;
	}
}

sjMatrixDouble sjLaplacianSmoothing::computeL(){
	sjMatrixDouble L;
	if (mesh_G.empty()) return L;
	size_t size = getNsize();
	L = sjMatrixDouble (size, sjVectorDouble(size,0));

	for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		sjPoint_3 point_i = v->point();
		vector< sjVertex_handle> vecinos = getRings()[v->index];

		double Wij = 0.0;

		int j_pos, j_Alpha_pos, j_Beta_pos;
		for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
			
			j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
			j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
			sjPoint_3 point_Alpha = vecinos[j_Alpha_pos]->point();
			sjPoint_3 point_Beta = vecinos[j_Beta_pos]->point();
			double Alpha = angle3(point_i, point_Alpha, vecinos[j_pos]->point());
			double Beta = angle3(point_i, point_Beta, vecinos[j_pos]->point());
			Wij = 1.0/ std::tan(Alpha) + 1.0/std::tan(Beta);
			L[v->index][vecinos[j_pos]->index] = Wij;
		}
		double  sum = 0;
		for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
			sum = sum - L[v->index][vecinos[j_pos]->index];
		}
		L[v->index][v->index] = sum;
	}

	return L;	
}

void sjLaplacianSmoothing::findRings(){
	rings.clear();
	for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		vector< sjVertex_handle> neighbors;
		do{
			sjVertex_handle punto = vcir->next()->vertex();
			neighbors.push_back(punto);
		}while(++vcir != v->vertex_begin ());
		rings.push_back(neighbors);
	}
}

sjVectorDouble sjLaplacianSmoothing::initRatioMatrixWl(double ratio){
	int n = (int)getNsize();
	sjVectorDouble matrixWl;
	double sqrtaverage = std::sqrt(averageFace());
	for(int i = 0; i<n; i++){
		//matrixWl.push_back(ratio * sqrtaverage);
		matrixWl.push_back(1.00);
	}
	return matrixWl;
}
sjVectorDouble sjLaplacianSmoothing::initRatioMatrixWh(double ratio){
	int n = (int)getNsize();
	sjVectorDouble matrixWh;
	for(int i = 0; i<n; i++){
		matrixWh.push_back(ratio);
	}
	return matrixWh;
}

double sjLaplacianSmoothing::averageFace(){
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

void sjLaplacianSmoothing::pruebaOpenNL(){
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

sjVectorDouble sjLaplacianSmoothing::areaRings(){
	int i, j, m, pos_1, pos_2, n = (int)(rings.size());
	double area;
	sjVectorDouble areas_vector;
	for(i=0; i<n; i++){
		vector< sjVertex_handle> neighbors = rings[i];
		m = neighbors.size();
		for(j=0; j<m; j++){
			pos_1 = j;
			pos_2 = (j+1) % m;
			sjPoint_3 point_i = sjPoint_3(matrix_V[i][0], matrix_V[i][1], matrix_V[i][2]);
			area = area3(point_i,neighbors[pos_1]->point(),neighbors[pos_2]->point());
			areas_vector.push_back(area);
		}
	}
	return areas_vector;
}

sjVectorDouble sjLaplacianSmoothing::updateMatrixWh(sjVectorDouble WH){
	sjVectorDouble mwh;
	int i, n = (int)(WH.size());
	for(i=0; i<n; i++){
		mwh.push_back( const_wh *  std::sqrt(  area_rings_original[i]/area_rings[i]   )   );
	}
	return mwh;
}

sjVectorDouble sjLaplacianSmoothing::updateMatrixWl(sjVectorDouble WL){
	sjVectorDouble mwl;
	int i, n = (int)(WL.size());
	for(i=0; i<n; i++){
		mwl.push_back(const_SL * WL[i]);
	}
	return mwl;
}

sjMatrixDouble sjLaplacianSmoothing::multiplyWlxL(sjVectorDouble WL, sjMatrixDouble L){
	size_t size = getNsize();
	sjMatrixDouble WlxL = sjMatrixDouble (size, sjVectorDouble(size,0));
	int i, j, nsize = (int)size;
	for(i=0; i<nsize; i++){
		for(j=0; j<nsize; j++){
			WlxL[i][j] = WL[i]* L[i][j] ;
		}
	}
	return WlxL;
}

sjVectorDouble sjLaplacianSmoothing::multiplyWhxV(sjVectorDouble WH, sjMatrixDouble V, int coord){
	size_t size = getNsize();
	sjVectorDouble WhxV = sjVectorDouble(size,0);
	int i, nsize = (int)size;
	for(i=0; i<nsize; i++){
		WhxV[i] = (WH[i])*(V[i][coord]);
	}
	return WhxV;
}

void sjLaplacianSmoothing::initLaplacianSystem(){
	cout<<"initLaplacianSystem 0."<<endl;
	initVertexIndex();
	cout<<"initLaplacianSystem 1."<<endl;
	findRings();
	cout<<"initLaplacianSystem 2."<<endl;
	area_rings_original = areaRings();
	cout<<"initLaplacianSystem 3."<<endl;
	matrix_Wl = initRatioMatrixWl(0.001);
	cout<<"initLaplacianSystem 4."<<endl;
	matrix_Wh = initRatioMatrixWh(1.0);
	cout<<"initLaplacianSystem 5."<<endl;
	matrix_L = computeL();
	cout<<"initLaplacianSystem 6."<<endl;
}

void sjLaplacianSmoothing::iterateLaplacianSystem(){
	cout<<"Paso 0."<<endl;
	if(iteration == 0){
		initLaplacianSystem();
	}
	cout<<"Paso 1."<<endl;

	sjMatrixDouble WlxL = multiplyWlxL(matrix_Wl, matrix_L);
	matrix_Vprima.clear();
	matrix_Vprima = sjMatrixDouble(getNsize(), sjVectorDouble(3, 0.0));

	cout<<"Matrix WlxL\n";
	printMatrix(WlxL);
	


	int coord;
	for(coord = 0; coord<3; coord++){

		sjVectorDouble WhxV = multiplyWhxV(matrix_Wh, matrix_V, coord);
		cout<<"Matrix WhxV\n";
		printVector(WhxV);


		

		int i, j;
		int n = (int)getNsize();


		cout<<"Paso 2."<<endl;
		sjLeastSquaresSolver solver(n);
		cout<<"Paso 3."<<endl;
		solver.set_least_squares(true) ;
		cout<<"Paso 4."<<endl;
		solver.begin_system() ;
		cout<<"Paso 5."<<endl;

		for(i=0; i<n; i++){	
			solver.begin_row();
			for(j=0; j<n; j++){	
				solver.add_coefficient(j, WlxL[i][j]);
			}
			solver.set_right_hand_side(0.0);
			solver.end_row();
		}
		cout<<"Paso 6."<<endl;
		for(i=0; i<n; i++){	
			solver.begin_row();
			solver.add_coefficient(i, matrix_Wh[i]);
			solver.set_right_hand_side(WhxV[i]);
			solver.end_row();
		}

		cout<<"Paso 6.2."<<endl;

		solver.end_system() ;
		cout<<"Paso 7."<<endl;
		if ( solver.solve() ){
			cout<<"Resolvio el sistema\n";
			for(i = 0; i< n; i++){
				cout<<i<<" = "<<solver.variable(i).value()<<"\t";
				matrix_Vprima[i][coord] = solver.variable(i).value();
			}
		}
	}

	cout<<"Original\n";
	printMatrix(matrix_V);
	cout<<"\nSolucion\n";
	printMatrix(matrix_Vprima);
	int iii = 0;
	sjPoint_3 promedio(0,0,0);
	for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		promedio = sjPoint_3(matrix_Vprima[iii][0],
								matrix_Vprima[iii][1],
								matrix_Vprima[iii][2]);	
		vcir->vertex()->point() = promedio;
		iii++;
	}

	iteration++;
}