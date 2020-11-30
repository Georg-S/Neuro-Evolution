#pragma once
#include "Genotype.h"
#include <vector>
#include <algorithm>

using namespace std;
namespace nev {

	class HistoricalGenotype
	{
	public:
		HistoricalGenotype(Innovation& inno, int countOfInputs, int countOfOutputs);
		HistoricalGenotype(Genotype genotype, int countOfInputs, int countOfOutputs);

		vector<double> calculateOutputSnapshotFromLastGenotype(const vector<double>& input);
		vector<double> calculateOutputActiveFromLastGenotype(const vector<double>& input);
		void setFitness(const double& fitness);
		void iterate(Innovation& inno);
		void evolution(Innovation& inno);
		void mutate(Innovation& inno, Genotype& genotype);


		friend bool operator<(HistoricalGenotype& lhs, HistoricalGenotype& rhs) {
			return lhs.getHighestFitness() > rhs.getHighestFitness();
		}

		void deletePhenotype();
		void purgeAllExceptHighestPerformingGenotype(Innovation& inno);
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
		void reset(Innovation& inno);
		Genotype getHighestPerformingGenotype(Innovation& inno);

		double highestFitnessAtStartOfCentury = 0;
		double improvementNeeded = 0.1;

		vector<Genotype> genotypeHistory;
		int countOfInputs;
		int countOfOutputs;
		double addLinkProbability = 0.05;
		double addNeuronProbability = 0.03;
		double mutateWeightProbability = 0.8;
		double newWeightProbability = 0.1;
		double maxWeightPertubation = 0.1;
		bool recurrentAllowed = true;
	};

}
