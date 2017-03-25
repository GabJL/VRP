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

