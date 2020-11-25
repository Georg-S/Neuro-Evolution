#pragma once
#include "Innovation.h"
#include <vector>
#include "LinkGene.h"
#include "NeuronGene.h"
#include "RNG.h"
#include <time.h>
#include <algorithm>
#include "Phenotype.h"
#include "ParentType.h"

using namespace std;

class Genotype
{
public:
	Genotype();
	Genotype(Innovation &innovation,int countOfInputs, int countOfOutputs, int id);
	Genotype(vector<NeuronGene> neurons, vector<LinkGene> links, int id);
	Genotype(Innovation &innovation, vector<NeuronGene> neurons, vector<LinkGene> links, int id);
	~Genotype();

	void randomlyAddNeuron(Innovation &innovation,float addNeuronProbability);
	void randomlyMutateAllWeights(float mutationProbability, float newWeightProbability, float weightPertubation);
	void randomlyAddLink(Innovation &innovation, float mutationProbability, bool recurrentAllowed);
	double calculateCompatibilityScore(Genotype &partner, const float &exzessFactor,
										const float &disjointFactor, const float &weightFactor);
	Genotype crossOver(Genotype & mother, int babyId);
	vector <double> calculateOutputSnapshot(const vector <double> &inputs);
	vector <double> calculateOutputActive(const vector <double> &inputs);
	void createPhenotype();
	void deletePhenotype();

	void setAdjustedFitness(double fitness);
	void setRawFitness(double fitness);
	double getAdjustedFitness();
	double getRawFitness();
	int getCountOfNeurons();
	int getCountOfHiddenNeurons();
	int getCountOfLinks();
	int getCountOfEnabledLinks();
	int getMaxDepth();
	double getLinkWeightAverage();
	int getGenotypeId();
	int getCountOfInputs();
	int getCountOfOutputs();
	vector<NeuronGene> getNeurons();
	vector<LinkGene> getLinks();

	friend bool operator<(const Genotype &lhs, const Genotype &rhs) {
		return lhs.rawFitness > rhs.rawFitness;
	}

	Phenotype* phenotype = nullptr;
private:
	void mutateSingleWeight(float newWeightProbability, int linkIndex, float weightPertubation);
	NeuronGene getNeuronGeneFromId(int id);
	void addLinkToVectorIfNotAlreadyInside(const LinkGene &link, vector<LinkGene> &linkVec);
	void addNeuronToVectorIfNotAlreadyInside(const NeuronGene &neuron, vector<NeuronGene> &neuronVec);
	void createFullyConnectedNetwork(Innovation &innovation);
	void createNeurons();
	void createLinks(Innovation &innovation);
	void calculateDepthOfEveryNeuron();
	void updateDepthOfNeuronsConnectedToThis(int neuronIndex);
	bool isValidLinkIndexForAddNeuron(int index);
	bool areValidNeuronIndizesForAddNeuron(int fromIndex, int toIndex, bool recurrentAllowed);
	bool doesLinkAlreadyExist(int fromIndex, int toIndex);
	bool isRecurrentBetweenNodes(int fromIndex, int toIndex);
	int getNeuronIndexFromId(int id);
	double getRandomLinkWeight();
	void createLinkWithRandomWeight(Innovation &innovation, int fromId, int toId, bool recurrent);
	void createLink(Innovation &innovation, int fromId, int toId, bool recurrent, double weightOfLink);

	vector <NeuronGene> neurons;
	vector <LinkGene> links;
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
