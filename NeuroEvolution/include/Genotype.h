#pragma once
#include <vector>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <memory>
#include "Innovation.h"
#include "LinkGene.h"
#include "NeuronGene.h"
#include "RNG.h"
#include "Phenotype.h"
#include "ParentType.h"


class Genotype
{
public:
	Genotype();
	Genotype(Innovation& innovation, const int& countOfInputs, const int& countOfOutputs, const int& id);
	Genotype(const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links, const int& id);
	Genotype(Innovation& innovation, const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links, const int& id);

	void randomlyAddNeuron(Innovation& innovation, const float& addNeuronProbability);
	void randomlyMutateAllWeights(const float& mutationProbability, const float& newWeightProbability, const double& weightPertubation);
	void randomlyAddLink(Innovation& innovation, const float& mutationProbability, const bool& recurrentAllowed);
	static double calculateCompatibilityScore(Genotype& left, Genotype& right, const float& exzessFactor, const float& disjointFactor, const float& weightFactor);
	static Genotype crossOver(const Genotype& father, const Genotype& mother, const int& babyId);
	std::vector <double> calculateOutputSnapshot(const std::vector <double>& inputs);
	std::vector <double> calculateOutputActive(const std::vector <double>& inputs);
	void createPhenotype();
	void deletePhenotype();

	void setAdjustedFitness(const double& fitness);
	void setRawFitness(const double& fitness);
	double getAdjustedFitness() const;
	double getRawFitness() const;
	int getCountOfNeurons() const;
	int getCountOfLinks() const;
	int getMaxDepth() const;
	double getLinkWeightAverage() const;
	int getGenotypeId() const;
	int getCountOfInputs() const;
	int getCountOfOutputs() const;
	std::vector<NeuronGene> getNeurons() const;
	std::vector<LinkGene> getLinks() const;
	friend bool operator<(const Genotype& lhs, const Genotype& rhs);

	std::shared_ptr<Phenotype> phenotype;

private:
	static ParentType getFittestParent(const Genotype& father, const Genotype& mother);
	void mutateSingleWeight(const float& newWeightProbability, LinkGene& link, const float& weightPertubation);
	NeuronGene getNeuronGeneFromId(const int& id) const;
	static NeuronGene getNeuronGeneFromId(const std::vector<NeuronGene>& neurons, const int& id);
	static void addLinkToVectorIfNotAlreadyInside(const LinkGene& link, std::vector<LinkGene>& linkVec);
	static void addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec);
	static void addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const int& linkIndex,
		std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec);
	static void addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const int& linkIndex, const double& weight,
		std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec);
	void createFullyConnectedNetwork(Innovation& innovation);
	void createNeurons();
	void createLinks(Innovation& innovation);
	void calculateDepthOfEveryNeuron();
	void updateDepthOfNeuronsConnectedToThis(NeuronGene& neuron);
	bool isValidLinkForAddNeuron(const LinkGene& link) const;
	bool areValidNeuronsForAddLink(const NeuronGene& fromNeuron, const NeuronGene& toNeuron, const bool& recurrentAllowed) const;
	bool doesLinkAlreadyExist(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const;
	bool isRecurrentBetweenNeurons(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const;
	int getNeuronIndexFromId(const int& id) const;
	static int getNeuronIndexFromId(const std::vector<NeuronGene>& neurons, const int& id);
	double getRandomLinkWeight() const;
	void createLinkWithRandomWeight(Innovation& innovation, const int& fromId, const int& toId, const bool& recurrent);
	void createLink(Innovation& innovation, const int& fromId, const int& toId, const bool& recurrent, const double& weightOfLink);

	std::vector <NeuronGene> neurons;
	std::vector <LinkGene> links;
	double rawFitness = 0;
	double adjustedFitness = 0;
	int numTriesToAddNeuron = 20;
	int numTriesToAddLink = 20;
	int id;
	int maxDepth = 0;
	int countOfInputs;
	int countOfOutputs;
	static constexpr int minimumLinkStartValue = -3;
	static constexpr int maximumLinkStartValue = 3;
	static constexpr double minimumLinkWeight = -30;
	static constexpr double maximumLinkWeight = 30;
};

