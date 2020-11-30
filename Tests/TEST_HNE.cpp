#include <gtest/gtest.h>
#include <HNE/HNE.h>
#include <vector>

using namespace std;
using namespace nev;

TEST(HNE, populationOf1CalculatesTheCorrectValue) {

	vector<HistoricalGenotype> histGenotype;
	vector<LinkGene> links;
	vector<NeuronGene> neurons;
	neurons.push_back(NeuronGene(NeuronType::bias, 0));
	neurons.push_back(NeuronGene(NeuronType::input, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	links.push_back(LinkGene(1, 2, 1, true, 0));
	Genotype geno = Genotype(neurons, links,0);
	geno.createPhenotype();

	histGenotype.push_back(HistoricalGenotype(geno, 1, 1));

	HNE epoch = HNE(histGenotype, 1, 1);

	vector<vector<double>> outputs = epoch.getOutputsSnapshot(vector<double>{1.0});

	EXPECT_DOUBLE_EQ(outputs[0][0], 0.99260845865571812);
}

TEST(HNE, populationOf2CalculatesTheCorrectValue) {

	vector<HistoricalGenotype> histGenotype;
	vector<LinkGene> links;
	vector<NeuronGene> neurons;
	neurons.push_back(NeuronGene(NeuronType::bias, 0));
	neurons.push_back(NeuronGene(NeuronType::input, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	links.push_back(LinkGene(1, 2, 1, true, 0));
	Genotype geno = Genotype(neurons, links,0);

	histGenotype.push_back(HistoricalGenotype(geno,1,1));
	histGenotype.push_back(HistoricalGenotype(geno,1,1));

	HNE epoch = HNE(histGenotype,1,1);

	vector<vector<double>> outputs = epoch.getOutputsSnapshot(vector<double>{1.0});
	
	for (int i = 0; i < outputs.size(); i++) {
		EXPECT_DOUBLE_EQ(outputs[i][0], 0.99260845865571812);
	}
}

TEST(HNE, populationOf3CalculatesTheCorrectValue) {
	vector<HistoricalGenotype> histGenotype;
	vector<LinkGene> links;
	vector<NeuronGene> neurons;
	neurons.push_back(NeuronGene(NeuronType::bias, 0));
	neurons.push_back(NeuronGene(NeuronType::input, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	links.push_back(LinkGene(1, 2, 1, true, 0));
	Genotype geno = Genotype(neurons, links,0);

	histGenotype.push_back(HistoricalGenotype(geno,1,1));
	histGenotype.push_back(HistoricalGenotype(geno,1,1));
	histGenotype.push_back(HistoricalGenotype(geno,1,1));

	HNE epoch = HNE(histGenotype,1,1);

	vector<vector<double>> outputs = epoch.getOutputsSnapshot(vector<double>{1.0});
	
	for (int i = 0; i < outputs.size(); i++) {
		EXPECT_DOUBLE_EQ(outputs[i][0], 0.99260845865571812);
	}
}


TEST(HNE, populationOf2GetsResetedAfterCenturyEnd) {
	HNE epoch = HNE(2,2,1, 1);
	epoch.setHighestFitnessAtStartOfCentury(1);
	epoch.iterateOneGeneration(vector<double>{1, 1});
	epoch.iterateOneGeneration(vector<double>{1, 1});
	epoch.iterateOneGeneration(vector<double>{1, 1});
}

TEST(HNE, calculateElitismCount0PercentReturnsCorrectResult) {
	HNE epoch = HNE(2, 10, 1, 1);
	epoch.setElitismPercentage(0);

	EXPECT_EQ(0, epoch.calculateElitismCount());
}
TEST(HNE, calculateElitismCount10PercentReturnsCorrectResult) {
	HNE epoch = HNE(2, 10, 1, 1);
	epoch.setElitismPercentage(0.1);
	
	EXPECT_EQ(1, epoch.calculateElitismCount());
}
TEST(HNE, calculateElitismCount33PercentReturnsCorrectResult) {
	HNE epoch = HNE(2, 10, 1, 1);
	epoch.setElitismPercentage(0.33);

	EXPECT_EQ(3, epoch.calculateElitismCount());
}

TEST(HNE, calculateElitismCount66PercentReturnsCorrectResult) {
	HNE epoch = HNE(2, 10, 1, 1);
	epoch.setElitismPercentage(0.66);

	EXPECT_EQ(6, epoch.calculateElitismCount());
}


TEST(HNE, theGenotypesGetPurgedIfNotEnoughImprovementIsMade) {
	HNE epoch = HNE(2, 2, 1, 1);
	epoch.setElitismPercentage(0.0);
	epoch.setHighestFitnessAtStartOfCentury(1);
	epoch.iterateOneGeneration(vector<double> {1, 1});
	epoch.iterateOneGeneration(vector<double> {1, 1});
	epoch.iterateOneGeneration(vector<double> {1, 1});

	EXPECT_EQ(0, epoch.getHighestFitness());
}

TEST(HNE, theGenotypesGetsNotPurgedIfEnoughImprovementIsMade) {
	HNE epoch = HNE(2, 2, 1, 1);;
	epoch.setElitismPercentage(0.0);
	epoch.setHighestFitnessAtStartOfCentury(1);
	epoch.iterateOneGeneration(vector<double> {1.2, 1.2});
	epoch.iterateOneGeneration(vector<double> {1.2, 1.2});
	epoch.iterateOneGeneration(vector<double> {1.2, 1.2});

	EXPECT_EQ(1.2, epoch.getHighestFitness());
}

TEST(HNE, theEliteDoesntGetPurgedAfterCenturyEnd) {
	HNE epoch = HNE(2, 2, 1, 1);;
	epoch.setElitismPercentage(0.5);
	epoch.setHighestFitnessAtStartOfCentury(1);
	epoch.iterateOneGeneration(vector<double> {1.05, 1.0});
	epoch.iterateOneGeneration(vector<double> {1.05, 1.0});
	epoch.iterateOneGeneration(vector<double> {1.05, 1.0});

	EXPECT_EQ(1.05, epoch.getHighestFitness());
}

TEST(HNE, theEliteDoesntGetPurgedAndReturnsTheSameOutput) {
	HNE epoch = HNE(3, 2, 1, 1);;
	epoch.setElitismPercentage(0.5);
	epoch.setHighestFitnessAtStartOfCentury(1);
	for (int i = 0; i < 100; i++) {
		vector <vector<double>> outputs = epoch.getOutputsSnapshot(vector<double> {1.0});
		double previousOutput = outputs[0][0];
		epoch.iterateOneGeneration(vector<double> {1.05, 1.0,1.0});
		epoch.iterateOneGeneration(vector<double> {1.00, 1.0,1.0});
		epoch.iterateOneGeneration(vector<double> {1.00, 1.0 , 1.0});
		outputs = epoch.getOutputsSnapshot(vector<double> {1.0});
		double currentOutput = outputs[0][0];

		EXPECT_EQ(previousOutput, currentOutput);
	}
}

TEST(Epoch, theOutputChangesAfterIteration) {
	HNE epoch = HNE(10, 1, 1, 1);;
	epoch.setMutateWeightProbability(1.0);
	vector<vector<double>> results = epoch.getOutputsSnapshot(vector<double>{1.0});
	double previousResult = results[0][0];
	epoch.iterateOneGeneration(vector<double> {1.0});
	results = epoch.getOutputsSnapshot(vector<double>{1.0});
	double currenResult = results[0][0];

	EXPECT_NE(previousResult, currenResult);
}