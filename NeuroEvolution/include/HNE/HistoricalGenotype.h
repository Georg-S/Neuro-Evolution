#pragma once
#include "Genotype.h"
#include <vector>
#include <algorithm>

namespace nev
{
	class HistoricalGenotype
	{
	public:
		HistoricalGenotype(Innovation* inno, int countOfInputs, int countOfOutputs);
		HistoricalGenotype(Genotype genotype, int countOfInputs, int countOfOutputs);
		std::vector<double> calculateOutputSnapshotFromLastGenotype(const std::vector<double>& input);
		std::vector<double> calculateOutputActiveFromLastGenotype(const std::vector<double>& input);
		void setFitness(const double& fitness);
		void iterate(Innovation* inno);
		void evolution(Innovation* inno);
		void mutate(Innovation* inno, Genotype& genotype);
		void deletePhenotype();
		void purgeAllExceptHighestPerformingGenotype(Innovation* inno);
		int getHistorySize() const;
		double getHighestFitness() const;
		double getHighestFitnessAtStartOfCentury() const;
		void setImprovementNeeded(double improvementNeeded);
		void setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury);
		void setAddLinkProbability(double addLinkProbability);
		void setAddNeuronProbability(double addNeuronProbability);
		void setMutateWeightProbability(double mutateWeightProbability);
		void setNewWeightProbability(double newWeightProbability);
		void setMaxWeightPertubation(double maxWeightPertubation);
		void setRecurrentAllowed(bool recurrentAllowed);
		friend bool operator<(const HistoricalGenotype& lhs, const HistoricalGenotype& rhs);

	private:
		bool improvedEnough() const;
		void reset(Innovation* inno);
		Genotype getHighestPerformingGenotype(Innovation* inno);

		double m_highestFitnessAtStartOfCentury = 0;
		double m_improvementNeeded = 0.1;
		std::vector<Genotype> m_genotypeHistory;
		int m_countOfInputs;
		int m_countOfOutputs;
		double m_addLinkProbability = 0.05;
		double m_addNeuronProbability = 0.03;
		double m_mutateWeightProbability = 0.8;
		double m_newWeightProbability = 0.1;
		double m_maxWeightPertubation = 0.1;
		bool m_recurrentAllowed = true;
	};
}
