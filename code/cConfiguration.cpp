#include <fstream>
#include <algorithm>
#include "cConfiguration.hpp"

using namespace std;

AlgorithmCfg::AlgorithmCfg(){
	runs = 30;
	evals = 100000;
	instance_name = "instances/vrpnc1.txt";
	experiment_name = "Experiment";
	verbose = true;
	neighborhood_operator = "Insertion";
	convergence = 5;
	popsize = 10;
	selection = "Random";
	mutationP = 0.8;
	mutationOp = "Insertion";
	recombinationP = 0.8;
	recombinationOp = "OX";
}

void AlgorithmCfg::readCfg(string filename){
	ifstream f(filename.c_str());
	string attr_name;
	unsigned value;

	if(!f.good()){
		perror("fichero de configuracion incorrecto");
		exit(-1);
	}
	f >> attr_name;
	while(!f.eof()){
		transform(attr_name.begin(), attr_name.end(), attr_name.begin(), ::tolower);
		if(attr_name == "runs") f >> runs;
		else if(attr_name == "evaluations") f >> evals; 
		else if(attr_name == "instance") f >> instance_name; 
		else if(attr_name == "experiment") f >> experiment_name; 
		else if(attr_name == "verbose"){ f >> value; verbose = (value == 1);}
		else if(attr_name == "neighbor") f >> neighborhood_operator;
		else if(attr_name == "convergence") f >> convergence;
		else if(attr_name == "popsize") f >> popsize;
		else if(attr_name == "selection") f >> selection;
		else if(attr_name == "mutationP") f >> mutationP;
		else if(attr_name == "mutationOp") f >> mutationOp;
		else if(attr_name == "recombinationP") f >> recombinationP;
		else if(attr_name == "recombinationOp") f >> recombinationOp;
		f >> ws >> attr_name;
	}
	f.close();
}

unsigned AlgorithmCfg::getRuns() const{
	return runs;
}

unsigned AlgorithmCfg::getEvaluations() const{
	return evals;
}

string AlgorithmCfg::getInstanceName() const{
	return instance_name;
}

string AlgorithmCfg::getExperimentName() const{
	return experiment_name;
}

bool AlgorithmCfg::isVerbose() const{
	return verbose;
}

string AlgorithmCfg::getNeighborhoodOperator() const{
	return neighborhood_operator;
}

unsigned AlgorithmCfg::getConvergence() const{
	return convergence;
}

unsigned AlgorithmCfg::getPopulationSize() const{
	return popsize;
}

string AlgorithmCfg::getSelection() const{
	return selection;
}

float AlgorithmCfg::getMutationProbability() const{
	return mutationP;
}

string AlgorithmCfg::getMutationOperator() const{
	return mutationOp;
}

float AlgorithmCfg::getRecombinationProbability() const{
	return recombinationP;
}

string AlgorithmCfg::getRecombinationOperator() const{
	return recombinationOp;
}
