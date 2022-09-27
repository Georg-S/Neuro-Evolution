#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <time.h>
#include "HistoricalGenotype.h"
#include "Innovation.h"


namespace nev
{
	class HNE
	{
	public:
		HNE(int centuryDuration, int populationSize, int countOfInputs, int countOfOutputs);
		HNE(const std::vector<nev::HistoricalGenotype>& population, int countOfInputs, int countOfOutputs);
		std::vector<std::vector<double>> getOutputsSnapshot(const std::vector<double>& inputs);
		void iterateOneGeneration(const std::vector<double>& fitness);
		std::vector<double> getOutputActiveByIndex(int index, const std::vector<double>& inputs);
		int calculateElitismCount();
		void setParametersOfPopulation();
		int getPopulationSize() const;
		double getHighestFitness() const;
		int getCurrentGeneration() const;
		void setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury);
		void setCenturyDuration(int centuryDuration);
		void setAddLinkProbability(double addLinkProbability);
		void setAddNeuronProbability(double addNeuronProbability);
		void setMutateWeightProbability(double mutateWeightProbability);
		void setNewWeightProbability(double newWeightProbability);
		void setMaxWeightPertubation(double maxWeightPertubation);
		void setRecurrentAllowed(bool recurrentAllowed);
		void setElitismPercentage(double elitismPercentage);

	private:
		std::vector<nev::HistoricalGenotype> m_population;
		int m_centuryDuration;
		int m_populationSize = 0;
		int m_countOfInputs = 0;
		int m_countOfOutputs = 0;
		std::unique_ptr<nev::Innovation> m_innovation;
		double m_elitismPercentage = 0.5;
		double m_addLinkProbability = 0.8;
		double m_addNeuronProbability = 0.5;
		double m_mutateWeightProbability = 0.8;
		double m_maxWeightPertubation = 0.1;
		double m_newWeightProbability = 0.1;
		bool m_recurrentAllowed = true;
		int m_currentCentury = 0;
		int m_currentGeneration = 0;
	};
}