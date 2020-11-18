#pragma once
#include <vector>
#include <time.h>
#include <algorithm>
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

	void randomlyAddNeuron(Innovation& innovation, const float &addNeuronProbability);
	void randomlyMutateAllWeights(float mutationProbability, float newWeightProbability, float weightPertubation);
	void randomlyAddLink(Innovation& innovation, float mutationProbability, bool recurrentAllowed);
	double calculateCompatibilityScore(Genotype& partner, const float& exzessFactor, const float& disjointFactor, const float& weightFactor);
	Genotype crossOver(Genotype& mother, int babyId);
	std::vector <double> calculateOutputSnapshot(const std::vector <double>& inputs);
	std::vector <double> calculateOutputActive(const std::vector <double>& inputs);
	void createPhenotype();
	void deletePhenotype();

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
	std::vector<NeuronGene> getNeurons() const;
	std::vector<LinkGene> getLinks() const;
	friend bool operator<(const Genotype& lhs, const Genotype& rhs);

	Phenotype* phenotype = nullptr;
private:
	void mutateSingleWeight(float newWeightProbability, int linkIndex, float weightPertubation);
	NeuronGene getNeuronGeneFromId(int id);
	void addLinkToVectorIfNotAlreadyInside(const LinkGene& link, std::vector<LinkGene>& linkVec);
	void addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec);
	void createFullyConnectedNetwork(Innovation& innovation);
	void createNeurons();
	void createLinks(Innovation& innovation);
	void calculateDepthOfEveryNeuron();
	void updateDepthOfNeuronsConnectedToThis(NeuronGene& neuron);
	bool isValidLinkIndexForAddNeuron(const int &index) const;
	bool areValidNeuronIndizesForAddNeuron(int fromIndex, int toIndex, bool recurrentAllowed);
	bool doesLinkAlreadyExist(int fromIndex, int toIndex);
	bool isRecurrentBetweenNodes(int fromIndex, int toIndex);
	int getNeuronIndexFromId(int id) const;
	double getRandomLinkWeight();
	void createLinkWithRandomWeight(Innovation& innovation, int fromId, int toId, bool recurrent);
	void createLink(Innovation& innovation, const int &fromId, const int &toId, const bool &recurrent, const double &weightOfLink);

	std::vector <NeuronGene> neurons;
	std::vector <LinkGene> links;
	double rawFitness = 0;
	double adjustedFitness = 0;
	int numTrysToAddNeuron = 20;
	int numTrysToAddLink = 20;
	int id;
	int maxDepth = 0;
	int countOfInputs;
	int countOfOutputs;
	int minimumLinkStartValue = -3;
	int maximumLinkStartValue = 3;
	int minimumLinkWeight = -30;
	int maximumLinkWeight = 30;
};

