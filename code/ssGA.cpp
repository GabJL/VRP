#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime> 
#include <algorithm>
#include "cInstance.hpp"

using namespace std;

typedef vector<unsigned> Solution;
typedef vector<Solution> Population;
const int POP_SIZE = 10;

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

void initializePopulation(Population &pop, vector<float> &fitness, const cInstance &c){
	pop.clear();
	for(int i = 0; i < POP_SIZE; i++){
		Solution s;
		generateSolution(s,c);
		fitness.push_back(evaluate(s,c));
		pop.push_back(s);
	}
}

void selectParents(const Population &p, Solution &i1, Solution &i2){
	int pos1 = rand()%(p.size()-1) + 1;
	if(rand()*1.0/RAND_MAX  > 0.5){
		i1 = p[pos1];
		i2 = p[rand()%pos1];
	} else {
		i1 = p[rand()%pos1];
		i2 = p[pos1];
	}
}

bool isIn(const Solution &s, const int start, const int end, const unsigned value){
	int i = start;
	while(i <= end && s[i] != value) i++;

	return i <= end;
}

void recombine(Solution &i1, const Solution &i2){
	int pos1, pos2; 
	do{
		pos1 = rand()%i1.size();	
		pos2 = rand()%i1.size();	
	}while(pos1 == pos2);
	if(pos2 < pos1){
		int aux = pos1;
		pos1 = pos2;
		pos2 = aux;
	}
	int j = 0;
	for(int i = 0; i < i1.size(); i++){
		if(i == pos1) i = pos2+1;
		if(i < i1.size()){
			while(j <  i1.size() && isIn(i1,pos1,pos2,i2[j])) j++;
			i1[i] = i2[j];
		}
		j++;
	}

}

void mutate(Solution &s, const cInstance &c){
	int pos1 = rand()%s.size();	
	int pos2 = rand()%s.size();	
	if(pos2 < pos1){
		int aux = pos1;
		pos1 = pos2;
		pos2 = aux;
	}

	while(pos1 < pos2){
		int city = s[pos1];
		s[pos1] = s[pos2];
		s[pos2] = city;	
		pos1++;
		pos2--;
	}
}

void merge(Population &p, const  Solution &s, vector<float> &fitness, const cInstance &c){
	float fit = evaluate(s,c);
	int pos = rand()%p.size();
	if(fitness[pos] > fit){
		fitness[pos] = fit;
		p[pos] = s;
	}
}

void algorithm(const cInstance &c, const unsigned steps, const bool verbose){
	Population population(POP_SIZE);
	vector<float> fitness;
	int pos;

	initializePopulation(population,fitness, c);
	if(verbose){
		pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
		cout << fitness[pos] << endl;
	}

	for(int i = POP_SIZE; i < steps; i++)
	{
		Solution ind1, ind2;
		selectParents(population, ind1, ind2);
		recombine(ind1, ind2);
		mutate(ind1,c);
		merge(population,ind1,fitness,c);
		if(verbose){
			pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
			cout << fitness[pos] << endl;
		}
	}

	pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
	if(verbose){
		cout << "Best solution: " << endl;
		for(int i = 0; i < population[pos].size(); i++)
			cout << population[pos][i] << " ";
		cout << endl << "Fitness: " << fitness[pos] << endl;
	} else {
		cout << fitness[pos] << endl;
	}
}

int main(int argc, char **argv)
{
	unsigned steps;
	unsigned runs;

	srand(time(0));

	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " <instance> <number of generations> <runs>" << endl;
		exit(-1);
	}

	cInstance c(argv[1]);
	steps = atoi(argv[2]);
	runs = atoi(argv[3]);

	for(int i = 0; i < runs ; i++)
	{
		algorithm(c, steps, runs == 1);
	}

	return 0;
}



