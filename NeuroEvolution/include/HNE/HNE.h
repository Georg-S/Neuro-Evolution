#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <time.h>
#include "HistoricalGenotype.h"
#include "Innovation.h"


namespace nev {

	class HNE
	{
	public:
		HNE(int centuryDuration, int populationSize, int countOfInputs, int countOfOutputs);
		HNE(std::vector<nev::HistoricalGenotype> population, int countOfInputs, int countOfOutputs);
		~HNE();

		std::vector<std::vector<double>> getOutputs(const std::vector<double>& inputs);
		void iterateOneGeneration(const std::vector<double>& fitness);
		std::vector<double> getOutputActiveByIndex(int index, const std::vector<double>& inputs);

		int calculateElitismCount();
		void setParametersOfPopulation();
		int getPopulationSize();
		void setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury);
		void setCenturyDuration(int centuryDuration);
		void setAddLinkProbability(double addLinkProbability);
		void setAddNeuronProbability(double addNeuronProbability);
		void setMutateWeightProbability(double mutateWeightProbability);
		void setNewWeightProbability(double newWeightProbability);
		void setMaxWeightPertubation(double maxWeightPertubation);
		void setRecurrentAllowed(bool recurrentAllowed);
		void setElitismPercentage(double elitismPercentage);
		double getHighestFitness();
		int getCurrentGeneration();
	private:
		double elitismPercentage = 0.5;
		double addLinkProbability = 0.8;
		double addNeuronProbability = 0.5;
		double mutateWeightProbability = 0.8;
		double maxWeightPertubation = 0.1;
		double newWeightProbability = 0.1;
		bool recurrentAllowed = true;
		int centuryDuration;
		int currentCentury = 0;
		int currentGeneration = 0;

		std::shared_ptr<nev::Innovation> innovation;
		std::vector<nev::HistoricalGenotype> population;
		int populationSize = 0;
		int countOfInputs = 0;
		int countOfOutputs = 0;
	};

}