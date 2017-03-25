#ifndef _ALG_CFG_
#define _ALG_CFG_

#include <string>

using namespace std;

class AlgorithmCfg{

public:
	AlgorithmCfg();

	void readCfg(string filename);

	unsigned getRuns() const;
	unsigned getEvaluations() const;
	string getInstanceName() const;
	string getExperimentName() const;
	bool isVerbose() const;
	string getNeighborhoodOperator() const;
	unsigned getConvergence() const;

private:
	unsigned runs;
	unsigned evals;
	string	instance_name;
	string	experiment_name;
	bool	verbose;	
	string 	neighborhood_operator;
	unsigned convergence;
};

#endif
