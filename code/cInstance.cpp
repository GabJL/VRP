#include "cInstance.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

cInstance::cInstance(string filename){
	vector<int> x;
	vector<int> y;
	ifstream is(filename.c_str());
	int xpos, ypos, dem;

	if(!is.good()){
		cout << "Fichero de instancia no encontrado" << endl;
		exit(-1);
	}

	is >> _nCustomers >> _capacity >> _maxRouteTime >> _service_time >> _best_cost;

	// Depot coordinates
	_demand.clear();
	is >> xpos >> ypos;
	x.push_back(xpos);
	y.push_back(ypos);
 	_demand.push_back(0);

 	// Read customers coordenates

	for(int i = 1; i < (_nCustomers + 1); i++){
		is >> xpos >> ypos >> dem;
		x.push_back(xpos);
		y.push_back(ypos);
 		_demand.push_back(dem);
	}

	genDistances(x,y);

}

int cInstance::nCustomers() const{
	return _nCustomers;
}

int cInstance::capacity() const{
	return _capacity;
}

double cInstance::maxRouteTime() const{
	return _maxRouteTime;
}

double cInstance::distance(const int i, const int j) const{
	return _distance[i][j];
}

int cInstance::demand(const int i) const{
	return _demand[i];
}

int cInstance::service_time() const{
	return _service_time;
}

double cInstance::best_cost() const{
	return _best_cost;
}

void cInstance::genDistances(const vector<int> &x,const vector<int> &y){
	for(int i = 0; i < (_nCustomers + 1); i++){
		vector<double> aux;
		for(int j = 0; j < (_nCustomers +1) ; j++)
			aux.push_back(sqrt( pow( (double) (x[j] - x[i]),2 ) +
				  pow( (double) (y[j] - y[i]),2 ) ) + _service_time);
		_distance.push_back(aux);
	}
}


