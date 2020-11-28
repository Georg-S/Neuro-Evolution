#pragma once
#include "Genotype.h"
#include <vector>
#include <algorithm>

using namespace std;

class HistoricalGenotype
{
public:
	HistoricalGenotype(int countOfInputs, int countOfOutputs);
	HistoricalGenotype(Genotype genotype, int countOfInputs, int countOfOutputs);
	~HistoricalGenotype();

	vector<double> calculateOutputSnapshotFromLastGenotype(const vector<double> &input);
	vector<double> calculateOutputActiveFromLastGenotype(const vector<double> &input);
	void setFitness(const double &fitness);
	void iterate();
	void evolution();
	void mutate(Genotype& genotype);


	friend bool operator<( HistoricalGenotype &lhs,  HistoricalGenotype &rhs) {
		return lhs.getHighestFitness() > rhs.getHighestFitness();
	}

	void deletePhenotype();
	void purgeAllExceptHighestPerformingGenotype();
	int getEnabedLinksCountFromLastGenotype();
	int getHiddenNodeCountFromLastGenotype();
	int getHistorySize();
	double getHighestFitness();
	double getHighestFitnessAtStartOfCentury();
	void setImprovementNeeded(double improvementNeeded);
	void setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury);
	void setAddLinkProbability(double addLinkProbability);
	void setAddNeuronProbability(double addNeuronProbability);
	void setMutateWeightProbability(double mutateWeightProbability);
	void setNewWeightProbability(double newWeightProbability);
	void setMaxWeightPertubation(double maxWeightPertubation);
	void setRecurrentAllowed(bool recurrentAllowed);
private:
	bool improvedEnough();
	void reset();
	Genotype getHighestPerformingGenotype();

	double highestFitnessAtStartOfCentury = 0;
	double improvementNeeded = 0.1;

	vector<Genotype> genotypeHistory;
	int countOfInputs;
	int countOfOutputs; 
	double addLinkProbability = 0.8;
	double addNeuronProbability = 0.5;
	double mutateWeightProbability = 0.8;
	double newWeightProbability = 0.1;
	double maxWeightPertubation = 0.1;
	bool recurrentAllowed = true;
};

