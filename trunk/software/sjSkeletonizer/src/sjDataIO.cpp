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

void sjDataIO::config(){
	int i = 0;
	//cout<<"Vertices: \n";
	for ( sjVIterator v1 = polyhedron_model.vertices_begin(); v1 != polyhedron_model.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		//cout<<"["<<i<<"] = "<<vcir->vertex()->point()<<"\n";
		i++;
	}
	cout<<"\n";
}

double sjDataIO::angle3(sjPoint_3 p1, sjPoint_3 p2, sjPoint_3 p3){
	sjVector_3 v1 = p1-p2;
	v1 = v1 / sqrt( v1 * v1);
	sjVector_3 v2 = p3-p2;
	v2 = v2 / sqrt( v2 * v2);
	//double anglex = std::acos( v1 * v2 ) / CGAL_PI * 180; 
	double anglex = std::acos( v1 * v2 ) ; 
	return anglex;
}

vector<vector<double>> sjDataIO::computeLaplacian(){
	vector<vector<double>> L;
	if (polyhedron_model.empty()) return L;
	size_t size = polyhedron_model.size_of_vertices();
	L = vector<vector<double>> (size, vector<double>(size,0));

	for ( sjVIterator v = polyhedron_model.vertices_begin(); v != polyhedron_model.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		sjPoint_3 point_i = v->point();
		vector< sjVertex_handle> vecinos;
		do{
			sjVertex_handle punto = vcir->next()->vertex();
			vecinos.push_back(punto);
		}while(++vcir != v->vertex_begin ());

		double Wij = 0.0;

		int j_pos, j_Alpha_pos, j_Beta_pos;
		//cout<<"Angulos\n";
		for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
			
			j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
			j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
			sjPoint_3 point_Alpha = vecinos[j_Alpha_pos]->point();
			sjPoint_3 point_Beta = vecinos[j_Beta_pos]->point();
			double Alpha = angle3(point_i, point_Alpha, vecinos[j_pos]->point());
			double Beta = angle3(point_i, point_Beta, vecinos[j_pos]->point());
			Wij = 1.0/ std::tan(Alpha) + 1.0/std::tan(Beta);
			//cout<<"i["<<v->index<<"], j["<<vecinos[j_pos]->index<<"] : ";
			//cout<<"Alpha = "<<Alpha<<", Beta"<<Beta<<"\n";
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

sjPolyhedron sjDataIO::getPolyhedronModel(){
	return polyhedron_model;
}