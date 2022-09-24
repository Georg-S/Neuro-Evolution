#pragma once
#include <vector>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <memory>
#include <functional>
#include "Innovation.h"
#include "LinkGene.h"
#include "NeuronGene.h"
#include "RNG.h"
#include "Phenotype.h"
#include "ActivationFunction.h"

namespace nev
{
	class Genotype
	{
	public:
		Genotype() = default;
		Genotype(Innovation* innovation, int countOfInputs, int countOfOutputs,
			int id, nev::af activationFunction = nev::af::steepenedSigmoid);
		Genotype(const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
			int id, nev::af activationFunction = nev::af::steepenedSigmoid);
		Genotype(Innovation* innovation, const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
			int id, nev::af activationFunction = nev::af::steepenedSigmoid);

		void randomlyAddNeuron(Innovation* innovation, double addNeuronProbability);
		void randomlyMutateAllWeights(double mutationProbability, double newWeightProbability, double weightPertubation);
		void randomlyAddLink(Innovation* innovation, double mutationProbability, bool recurrentAllowed);
		static double calculateCompatibilityScore(Genotype* left, Genotype* right, double excessFactor,
			double disjointFactor, double weightFactor);
		static std::shared_ptr<Genotype> crossOver(Genotype* father, Genotype* mother, int babyId);
		std::vector <double> calculateOutputSnapshot(const std::vector <double>& inputs);
		std::vector <double> calculateOutputActive(const std::vector <double>& inputs);
		void createPhenotype();
		void deletePhenotype();

		void setActivationFunction(nev::af activationFunction);
		void setAdjustedFitness(double fitness);
		void setRawFitness(double fitness);
		double getAdjustedFitness() const;
		double getRawFitness() const;
		int getCountOfNeurons() const;
		int getCountOfLinks() const;
		int getMaxDepth() const;
		double getLinkWeightAverage() const;
		int getGenotypeId() const;
		int getCountOfInputs() const;
		int getCountOfOutputs() const;
		int getCountOfEnabledLink() const;
		int getCountOfHiddenNeurons() const;
		std::vector<NeuronGene> getNeurons() const;
		std::vector<LinkGene> getLinks() const;
		friend bool operator<(const Genotype& lhs, const Genotype& rhs);

		std::shared_ptr<Phenotype> phenotype;

	private:
		enum class ParentType
		{
			Mother, Father
		};
		static ParentType getFittestParent(const Genotype& father, const Genotype& mother);
		void mutateSingleWeight(LinkGene* link, double newWeightProbability, double weightPertubation);
		NeuronGene getNeuronGeneFromId(int id) const;
		static NeuronGene getNeuronGeneFromId(const std::vector<NeuronGene>& neurons, int id);
		static void addLinkToVectorIfNotAlreadyInside(const LinkGene& link, std::vector<LinkGene>& linkVec);
		static void addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec);
		static void addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const LinkGene& link,
			std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec);
		static void addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const LinkGene& link, double weight,
			std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec);
		void createFullyConnectedNetwork(Innovation* innovation);
		void createNeurons();
		void createLinks(Innovation* innovation);
		void calculateDepthOfEveryNeuron();
		void updateDepthOfNeuronsConnectedToThis(const NeuronGene& neuron);
		bool isValidLinkForAddNeuron(const LinkGene& link) const;
		bool areValidNeuronsForAddLink(const NeuronGene& fromNeuron, const NeuronGene& toNeuron, bool recurrentAllowed) const;
		bool doesLinkAlreadyExist(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const;
		bool isRecurrentBetweenNeurons(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const;
		int getNeuronIndexFromId(int id) const;
		static int getNeuronIndexFromId(const std::vector<NeuronGene>& neurons, int id);
		double getRandomLinkWeight() const;
		void createLinkWithRandomWeight(Innovation* innovation, int fromId, int toId, bool recurrent);
		void createLink(Innovation* innovation, int fromId, int toId, bool recurrent, double weightOfLink);
		std::vector<LinkGene> getAllValidLinksForAddNeuron();
		void disableLink(const LinkGene& linkToDisable);

		int m_countOfInputs;
		int m_countOfOutputs;
		std::vector <NeuronGene> m_neurons;
		std::vector <LinkGene> m_links;	// TODO: can probably refactored to be a map
		int m_id;
		nev::af m_activationFunction;
		double m_rawFitness = 0;
		double m_adjustedFitness = 0;
		int m_maxDepth = 0;
		static const int NumTriesToAddLink = 20;
		static constexpr int MinimumLinkStartValue = -3;
		static constexpr int MaximumLinkStartValue = 3;
		static constexpr double MinimumLinkWeight = -30;
		static constexpr double MaximumLinkWeight = 30;
	};

}