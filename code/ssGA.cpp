#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime> 
#include <algorithm>
#include "cInstance.hpp"
#include "cConfiguration.hpp"

using namespace std;

typedef vector<unsigned> Solution;
typedef vector<Solution> Population;

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

void initializePopulation(Population &pop, vector<float> &fitness, const cInstance &c, const unsigned popSize){
	pop.clear();
	for(int i = 0; i < popSize; i++){
		Solution s;
		generateSolution(s,c);
		fitness.push_back(evaluate(s,c));
		pop.push_back(s);
	}
}

void randomSelection(const Population &p, Solution &i){
	i = p[rand()%p.size()];
}

void binaryTournament(const Population &p, const vector<float> &fitness, Solution &i){
	int pos1 = rand()%(p.size()-1) + 1;
	int pos2 = rand()%pos1;

	if(fitness[pos1] >= fitness[pos2]) i = p[pos1];
	else i = p[pos2];
}

void selectParent(const Population &p, const vector<float> &fitness, Solution &i, const string &selM){
	if(selM == "BinTour")  binaryTournament(p,fitness, i);
	else					randomSelection(p,i);
}


bool isIn(const Solution &s, const int start, const int end, const unsigned value){
	int i = start;
	while(i <= end && s[i] != value) i++;

	return i <= end;
}

void OX(Solution &i1, const Solution &i2){
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

void CX(Solution &i1, const Solution &i2){
		Solution s = i1;
		int max = i1.size();
		vector<bool> elegidos(max);
		vector<int> pos1(max);
		vector<int> pos2(max);
		bool fin = false;
		int elemento;
		int parent;

		// pos?[i] = j -> si en la solucion? el cliente i+1 se visita en j-ésimo lugar.
		for (int i = 0; i < max; i++)
		{
			i1[i] = 0;
			elegidos[i] = false;
			pos1[s[i]-1] = i;
			pos2[i2[i]-1] = i;
		}

		parent = 0;
		for (int i = 0 ; i < max; i++)
		{
			if(!elegidos[i])
			{
				i1[i] = (parent == 0?s[i]:i2[i]);
				elemento = i1[i]-1;
				elegidos[i] = true;
				do
				{
					int pos = ((1-parent)==0?pos1[elemento]:pos2[elemento]);
                    fin = elegidos[pos];
                    if(!fin)
                    {
						i1[pos] = (parent==0?s[pos]:i2[pos]);
						elemento = 	i1[pos]-1;
						elegidos[pos] = true;
                    }

				} while(!fin);
				parent = 1 - parent;
			}
		}
}

void recombine(Solution &i1, const Solution &i2, const float probC, const string &op){
	int prob = rand()*1.0/RAND_MAX;
	if(prob <= probC){
		if(op == "CX") 	CX(i1,i2);
		else OX(i1,i2);
	}
}

void insertion(Solution &sol){
	int pos1 = rand()%sol.size();		
	int city = sol[pos1];				
	sol.erase(sol.begin() + pos1);

	int pos2 = rand()%sol.size();
	sol.insert(sol.begin()+pos2,city); 
	
}

void interchange(Solution &sol){
	int pos1 = rand()%sol.size();
	int pos2 = rand()%sol.size();	

	int city = sol[pos1];		
	sol[pos1] = sol[pos2];
	sol[pos2] = city;
	
}

void inversion(Solution &s){
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

void mutate(Solution &s, const float &probM, const string &op){
	int prob = rand()*1.0/RAND_MAX;
	if(prob <= probM){
		if(op == "Interchange") 	interchange(s);
		else if(op == "Inversion") 	inversion(s);
		else  						insertion(s);
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

void algorithm(const cInstance &c, const AlgorithmCfg &cfg, const unsigned run){
	Population population(cfg.getPopulationSize());
	vector<float> fitness;
	int pos;

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

	initializePopulation(population,fitness, c, cfg.getPopulationSize());
	if(cfg.isVerbose()) {
		pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
		evals << fitness[pos] << endl;
	}

	for(int i = cfg.getPopulationSize(); i < cfg.getEvaluations(); i++)
	{
		Solution ind1, ind2;
		selectParent(population, fitness, ind1, cfg.getSelection());
		selectParent(population, fitness, ind2, cfg.getSelection());
		recombine(ind1, ind2, cfg.getRecombinationProbability(), cfg.getRecombinationOperator());
		mutate(ind1,cfg.getMutationProbability(), cfg.getMutationOperator());
		merge(population,ind1,fitness,c);
		if(cfg.isVerbose()) {
			pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
			evals << fitness[pos] << endl;
		}
	}

	pos = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
	res << fitness[pos] << endl;

	res.close();
	evals.close();

}

int main(int argc, char **argv)
{
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



