#pragma once
#include <vector>
#include "LinkGene.h"
#include "NeuronGene.h"
#include "NeuronType.h"
#include "RNG.h"
#include "Phenotype.h"
#include "PhenotypeNeuron.h"
#include "PhenotypeLink.h"

using namespace std;

class Genotype
{
public:
	Phenotype* phenotype;

	Genotype();
	Genotype(int countOfInputs, int countOfOutputs);
	Genotype(vector < NeuronGene> neurons, vector<LinkGene> links);
	~Genotype();

	friend bool operator<(Genotype &lhs, Genotype &rhs) {
		return lhs.fitness > rhs.fitness;
	}

	void setFitness(double fitness);
	void setBiasRandomWeight(int minimumWeight, int maximumWeight);
	void setOtherNeuronsRandomWeight(int minimumWeight, int maximumWeight);
	void setTrysToAddLink(int trysToAddLink);
	double getFitness();
	int getCountOfNeurons();
	int getCountOfLinks();
	int getMaxDepth();
	double getLinkWeightAverage();
	int getHiddenNodeCount();
	int getEnabledLinksCount();


	void randomlyAddNeuron(double addNeuronProbability);
	void randomlyAddLink(double addLinkProbability, bool recurrentAllowed);
	void randomlyMutateWeights(double mutationProbability,double newValueProbability, double maxMutationPertubation);
	vector<double> calculateOutputSnapshot(const vector<double> &input);
	vector<double> calculateOutputActive(const vector<double> &input);
	void createPhenotype();
	void deletePhenotype();
private:
	vector<NeuronGene> neurons;
	vector<LinkGene> links;
	int trysToAddLink = 10;
	double fitness = 0;
	int maxDepth;
	int countOfInputs;
	int countOfOutputs;

	int biasRandomMinimumWeight = -3;
	int biasRandomMaximumWeight = 3;
	int otherNeuronsRandomMinimumWeight = -3;
	int otherNeuronsRandomMaximumWeight = 3;


	void createFullyConnectedGenotype();
	void createNeurons();
	void createLinks();
	void calculateDepthOfEveryNeuron();
	void updateDepthOfNeuronsConnectedToThis(int neuronIndex);
	bool isValidLinkForAddNeuron(int linkIndex);
	bool isRecurrentBetweenNodes(int fromIndex, int toIndex);
	bool areValidIndizesForAddLink(int fromIndex, int toIndex, bool recurrentAllowed);
	bool doesLinkAlreadyExist(int fromIndex, int toIndex);
	int getNeuronIndexFromId(int id);
	float getRandomLinkWeight(bool bias);
};

