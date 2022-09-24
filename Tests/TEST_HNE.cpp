#include <gtest/gtest.h>
#include <HNE/HNE.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_HNE, populationOf1CalculatesTheCorrectValue) {

	std::vector<nev::HistoricalGenotype> histGenotype;
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	links.push_back(nev::LinkGene(1, 2, 1, true, 0));
	nev::Genotype geno = nev::Genotype(neurons, links,0);
	geno.createPhenotype();

	histGenotype.push_back(nev::HistoricalGenotype(geno, 1, 1));

	nev::HNE hne = nev::HNE(histGenotype, 1, 1);

	std::vector<std::vector<double>> outputs = hne.getOutputsSnapshot(std::vector<double>{1.0});

	EXPECT_DOUBLE_EQ(outputs[0][0], 0.99260845865571812);
}

TEST(TEST_HNE, populationOf2CalculatesTheCorrectValue) {

	std::vector<nev::HistoricalGenotype> histGenotype;
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	links.push_back(nev::LinkGene(1, 2, 1, true, 0));
	nev::Genotype geno = nev::Genotype(neurons, links,0);

	histGenotype.push_back(nev::HistoricalGenotype(geno,1,1));
	histGenotype.push_back(nev::HistoricalGenotype(geno,1,1));

	nev::HNE hne = nev::HNE(histGenotype,1,1);

	std::vector<std::vector<double>> outputs = hne.getOutputsSnapshot(std::vector<double>{1.0});
	
	for (int i = 0; i < outputs.size(); i++)
		EXPECT_DOUBLE_EQ(outputs[i][0], 0.99260845865571812);
}

TEST(TEST_HNE, populationOf3CalculatesTheCorrectValue) {
	std::vector<nev::HistoricalGenotype> histGenotype;
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	links.push_back(nev::LinkGene(1, 2, 1, true, 0));
	nev::Genotype geno = nev::Genotype(neurons, links,0);

	histGenotype.push_back(nev::HistoricalGenotype(geno,1,1));
	histGenotype.push_back(nev::HistoricalGenotype(geno,1,1));
	histGenotype.push_back(nev::HistoricalGenotype(geno,1,1));

	nev::HNE hne = nev::HNE(histGenotype,1,1);

	std::vector<std::vector<double>> outputs = hne.getOutputsSnapshot(std::vector<double>{1.0});
	
	for (int i = 0; i < outputs.size(); i++)
		EXPECT_DOUBLE_EQ(outputs[i][0], 0.99260845865571812);
}


TEST(TEST_HNE, populationOf2GetsResetedAfterCenturyEnd) {
	nev::HNE hne = nev::HNE(2,2,1, 1);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration(std::vector<double>{1, 1});
	hne.iterateOneGeneration(std::vector<double>{1, 1});
	hne.iterateOneGeneration(std::vector<double>{1, 1});
}

TEST(TEST_HNE, calculateElitismCount0PercentReturnsCorrectResult) {
	nev::HNE hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0);

	EXPECT_EQ(0, hne.calculateElitismCount());
}
TEST(TEST_HNE, calculateElitismCount10PercentReturnsCorrectResult) {
	nev::HNE hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.1);
	
	EXPECT_EQ(1, hne.calculateElitismCount());
}
TEST(TEST_HNE, calculateElitismCount33PercentReturnsCorrectResult) {
	nev::HNE hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.33);

	EXPECT_EQ(3, hne.calculateElitismCount());
}

TEST(TEST_HNE, calculateElitismCount66PercentReturnsCorrectResult) {
	nev::HNE hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.66);

	EXPECT_EQ(6, hne.calculateElitismCount());
}


TEST(TEST_HNE, theGenotypesGetPurgedIfNotEnoughImprovementIsMade) {
	nev::HNE hne = nev::HNE(2, 2, 1, 1);
	hne.setElitismPercentage(0.0);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration(std::vector<double> {1, 1});
	hne.iterateOneGeneration(std::vector<double> {1, 1});
	hne.iterateOneGeneration(std::vector<double> {1, 1});

	EXPECT_EQ(0, hne.getHighestFitness());
}

TEST(TEST_HNE, theGenotypesGetsNotPurgedIfEnoughImprovementIsMade) {
	nev::HNE hne = nev::HNE(2, 2, 1, 1);;
	hne.setElitismPercentage(0.0);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration(std::vector<double> {1.2, 1.2});
	hne.iterateOneGeneration(std::vector<double> {1.2, 1.2});
	hne.iterateOneGeneration(std::vector<double> {1.2, 1.2});

	EXPECT_EQ(1.2, hne.getHighestFitness());
}

TEST(TEST_HNE, theEliteDoesntGetPurgedAfterCenturyEnd) {
	nev::HNE hne = nev::HNE(2, 2, 1, 1);;
	hne.setElitismPercentage(0.5);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration(std::vector<double> {1.05, 1.0});
	hne.iterateOneGeneration(std::vector<double> {1.05, 1.0});
	hne.iterateOneGeneration(std::vector<double> {1.05, 1.0});

	EXPECT_EQ(1.05, hne.getHighestFitness());
}

TEST(TEST_HNE, theEliteDoesntGetPurgedAndReturnsTheSameOutput) {
	nev::HNE hne = nev::HNE(3, 2, 1, 1);;
	hne.setElitismPercentage(0.5);
	hne.setHighestFitnessAtStartOfCentury(1);
	for (int i = 0; i < 100; i++) {
		std::vector <std::vector<double>> outputs = hne.getOutputsSnapshot(std::vector<double> {1.0});
		double previousOutput = outputs[0][0];
		hne.iterateOneGeneration(std::vector<double> {1.05, 1.0,1.0});
		hne.iterateOneGeneration(std::vector<double> {1.00, 1.0,1.0});
		hne.iterateOneGeneration(std::vector<double> {1.00, 1.0 , 1.0});
		outputs = hne.getOutputsSnapshot(std::vector<double> {1.0});
		double currentOutput = outputs[0][0];

		EXPECT_EQ(previousOutput, currentOutput);
	}
}

TEST(TEST_HNE, theOutputChangesAfterIteration) {
	nev::HNE hne = nev::HNE(10, 1, 1, 1);;
	hne.setMutateWeightProbability(1.0);
	hne.setNewWeightProbability(1.0);
	std::vector<std::vector<double>> results = hne.getOutputsSnapshot(std::vector<double>{1.0});
	double previousResult = results[0][0];
	hne.iterateOneGeneration(std::vector<double> {1.0});
	results = hne.getOutputsSnapshot(std::vector<double>{1.0});
	double currenResult = results[0][0];

	EXPECT_NE(previousResult, currenResult);
}

#endif