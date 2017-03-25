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
	unsigned getPopulationSize() const;
	string getSelection() const;
	float getMutationProbability() const;
	string getMutationOperator() const;
	float getRecombinationProbability() const;
	string getRecombinationOperator() const;

private:
	unsigned runs;
	unsigned evals;
	string	instance_name;
	string	experiment_name;
	bool	verbose;	
	string 	neighborhood_operator;
	unsigned convergence;
	unsigned popsize;
	string	 selection;
	float	mutationP;
	string 	mutationOp;
	float	recombinationP;
	string 	recombinationOp;
};

#endif
