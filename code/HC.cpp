#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime> 
#include "cInstance.hpp"
#include "cConfiguration.hpp"

using namespace std;

typedef vector<unsigned> Solution;

void generateSolution(Solution &solution, const cInstance &c){
	unsigned aux,ind1,ind2;
	unsigned max = c.nCustomers();

	solution.clear();

	// Create Ordered Array from 0 to nCustomers
	for(int i = 0; i < max; i++)
		solution.push_back(i+1);


	// move values to create random array
	for(int i = 0; i < (max*5) ; i++){
		ind1 = rand()%max;
		ind2 = rand()%max;

		aux = solution[ind1];
		solution[ind1] = solution[ind2];
		solution[ind2] = aux;
	}
}

double evaluate(const Solution &solution, const cInstance &c){
	double fitness = 0.0;
	int ultimo = 0;
	double dist = 0.0;
	int cap = c.capacity();

	register int c_actual;

	for(int j = 0; j < c.nCustomers(); j++) {
		c_actual = solution[j];

		if( ((dist + c.distance(ultimo,c_actual) + c.distance(c_actual,0)) >  c.maxRouteTime())
		 || (( cap - c.demand(c_actual)) < 0)
		// No solo mete una nueva ruta cuando se pasa sino cuando crear una nueva es mejor
		 || ((c.distance(ultimo,0)  + c.distance(0,c_actual)) < c.distance(ultimo,c_actual))
		 ) {
			fitness += dist + c.distance(ultimo,0);
			ultimo = 0;
			dist = 0.0;
			cap = c.capacity();
		}
		else {
			dist += c.distance(ultimo,c_actual);
			cap -= c.demand(c_actual);
			ultimo = c_actual;
		}
	}

	fitness += dist + c.distance(ultimo,0);

	return fitness;
}

void insertion(const Solution &sol, Solution &neigh){
	neigh = sol;						// Copiamos la actual al vecino
	
	int pos1 = rand()%neigh.size();		// Posición a mover
	int city = neigh[pos1];				// Tomamos la ciudad a mover
	neigh.erase(neigh.begin() + pos1);	// Lo borramos de la solución

	int pos2 = rand()%neigh.size();		// Posición donde insertar
	neigh.insert(neigh.begin()+pos2,city); // La insertamos en la nueva posición
	
}

void interchange(const Solution &sol, Solution &neigh){
	neigh = sol;
	
	int pos1 = rand()%neigh.size();
	int pos2 = rand()%neigh.size();	

	int city = neigh[pos1];		
   neigh[pos1] = neigh[pos2];
	neigh[pos2] = city;
	
}

void inversion(const Solution &sol, Solution &neigh){
	neigh = sol;
	int pos1 = rand()%neigh.size();	
	int pos2 = rand()%neigh.size();	
	if(pos2 < pos1){
		int aux = pos1;
		pos1 = pos2;
		pos2 = aux;
	}

	while(pos1 < pos2){
		int city = neigh[pos1];
		neigh[pos1] = neigh[pos2];
		neigh[pos2] = city;	
		pos1++;
		pos2--;
	}
}

void generateNeighbor(const Solution &sol, Solution &neigh, const string &op){
	if(op == "Interchange") 		interchange(sol, neigh);
	else if(op == "Inversion") 	inversion(sol, neigh);
	else  								insertion(sol, neigh);
}

void algorithm(const cInstance &c, const AlgorithmCfg &cfg, const unsigned run){
	Solution current_sol, best_sol;
	double fitness, best_fit;

	// For writing the results;
	ofstream res, evals;
	string fname1 = cfg.getExperimentName() + "g.txt"; 
	string fname2 = cfg.getExperimentName() + to_string(run) + ".txt"; 
	if(run == 0){
		res.open(fname1.c_str());
	} else {
		res.open(fname1.c_str(),ios::app);
	}
	evals.open(fname2.c_str());

	generateSolution(current_sol, c);
	fitness = evaluate(current_sol, c);
	best_sol = current_sol; best_fit = fitness;
	if(cfg.isVerbose())  evals << best_fit << endl;

	for(int i = 1; i < cfg.getEvaluations(); i++)
	{
		generateNeighbor(best_sol, current_sol, cfg.getNeighborhoodOperator());
		fitness = evaluate(current_sol, c);
		if(fitness < best_fit)
		{
			best_sol = current_sol; 
			best_fit = fitness;
		}
		if(cfg.isVerbose())  evals << best_fit << endl;
	}

	res << best_fit << endl;

	res.close();
	evals.close();
}

int main(int argc, char **argv){

	srand(time(0));

	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <configuration file>" << endl;
		exit(-1);
	}

	AlgorithmCfg cfg;
	cfg.readCfg(argv[1]);
	cInstance c(cfg.getInstanceName());

	for(int i = 0; i < cfg.getRuns() ; i++)
	{
		algorithm(c, cfg, i);
	}

	return 0;
}


