#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime> 
#include "cInstance.hpp"

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

void RS(const cInstance &c, const unsigned steps, const bool verbose){
	Solution current_sol, best_sol;
	double fitness, best_fit;

	generateSolution(current_sol, c);
	fitness = evaluate(current_sol, c);
	best_sol = current_sol; best_fit = fitness;
	if(verbose)  cout << fitness << endl;

	for(int i = 1; i < steps; i++)
	{
		generateSolution(current_sol, c);
		fitness = evaluate(current_sol, c);
		if(verbose)  cout << fitness << endl;
		if(fitness < best_fit)
		{
			best_sol = current_sol; 
			best_fit = fitness;
		}
	}

	if(verbose){
		cout << "Best solution: " << endl;
		for(int i = 0; i < best_sol.size(); i++)
			cout << best_sol[i] << " ";
		cout << endl << "Fitness: " << best_fit << endl;
	} else {
		cout << best_fit << endl;
	}
}

int main(int argc, char **argv){

	unsigned steps, runs;

	srand(time(0));

	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " <instance> <number of iterations> <runs>" << endl;
		exit(-1);
	}

	cInstance c(argv[1]);
	steps = atoi(argv[2]);
	runs = atoi(argv[3]);

	for(int i = 0; i < runs ; i++)
	{
		RS(c, steps, runs == 1);
	}

	return 0;
}


