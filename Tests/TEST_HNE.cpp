#include <gtest/gtest.h>
#include <HNE/HNE.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_HNE, populationOf1CalculatesTheCorrectValue)
{
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.emplace_back(nev::NeuronType::bias, 0);
	neurons.emplace_back(nev::NeuronType::input, 1);
	neurons.emplace_back(nev::NeuronType::output, 2);
	links.emplace_back(1, 2, 1, true, 0);
	auto geno = nev::Genotype(std::move(neurons), std::move(links), 0);

	std::vector<nev::HistoricalGenotype> histGenotype;
	histGenotype.emplace_back(geno, 1, 1);

	auto hne = nev::HNE(std::move(histGenotype), 1, 1);
	auto outputs = hne.getOutputsSnapshot({1.0});

	EXPECT_DOUBLE_EQ(outputs[0][0], 0.99260845865571812);
}

TEST(TEST_HNE, populationOf2CalculatesTheCorrectValue)
{
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.emplace_back(nev::NeuronType::bias, 0);
	neurons.emplace_back(nev::NeuronType::input, 1);
	neurons.emplace_back(nev::NeuronType::output, 2);
	links.emplace_back(1, 2, 1, true, 0);
	auto geno = nev::Genotype(std::move(neurons), std::move(links), 0);

	std::vector<nev::HistoricalGenotype> histGenotype;
	histGenotype.emplace_back(geno, 1, 1);
	histGenotype.emplace_back(geno, 1, 1);

	auto hne = nev::HNE(std::move(histGenotype), 1, 1);

	auto outputs = hne.getOutputsSnapshot({1.0});

	for(const auto& output : outputs)
		EXPECT_DOUBLE_EQ(output[0], 0.99260845865571812);
}

TEST(TEST_HNE, populationOf3CalculatesTheCorrectValue)
{
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.emplace_back(nev::NeuronType::bias, 0);
	neurons.emplace_back(nev::NeuronType::input, 1);
	neurons.emplace_back(nev::NeuronType::output, 2);
	links.emplace_back(1, 2, 1, true, 0);
	auto geno = nev::Genotype(std::move(neurons), std::move(links), 0);

	std::vector<nev::HistoricalGenotype> histGenotype;
	histGenotype.emplace_back(nev::HistoricalGenotype(geno, 1, 1));
	histGenotype.emplace_back(nev::HistoricalGenotype(geno, 1, 1));
	histGenotype.emplace_back(nev::HistoricalGenotype(geno, 1, 1));

	auto hne = nev::HNE(std::move(histGenotype), 1, 1);

	auto outputs = hne.getOutputsSnapshot({1.0});

	for (const auto& output : outputs)
		EXPECT_DOUBLE_EQ(output[0], 0.99260845865571812);
}

TEST(TEST_HNE, calculateElitismCount0PercentReturnsCorrectResult)
{
	auto hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0);

	EXPECT_EQ(0, hne.calculateElitismCount());
}
TEST(TEST_HNE, calculateElitismCount10PercentReturnsCorrectResult)
{
	auto hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.1);

	EXPECT_EQ(1, hne.calculateElitismCount());
}
TEST(TEST_HNE, calculateElitismCount33PercentReturnsCorrectResult)
{
	auto hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.33);

	EXPECT_EQ(3, hne.calculateElitismCount());
}

TEST(TEST_HNE, calculateElitismCount66PercentReturnsCorrectResult)
{
	auto hne = nev::HNE(2, 10, 1, 1);
	hne.setElitismPercentage(0.66);

	EXPECT_EQ(6, hne.calculateElitismCount());
}


TEST(TEST_HNE, theGenotypesGetPurgedIfNotEnoughImprovementIsMade)
{
	auto hne = nev::HNE(2, 2, 1, 1);
	hne.setElitismPercentage(0.0);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration({1, 1});
	hne.iterateOneGeneration({1, 1});
	hne.iterateOneGeneration({1, 1});

	EXPECT_EQ(0, hne.getHighestFitness());
}

TEST(TEST_HNE, theGenotypesGetsNotPurgedIfEnoughImprovementIsMade)
{
	auto hne = nev::HNE(2, 2, 1, 1);;
	hne.setElitismPercentage(0.0);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration({1.2, 1.2});
	hne.iterateOneGeneration({1.2, 1.2});
	hne.iterateOneGeneration({1.2, 1.2});

	EXPECT_EQ(1.2, hne.getHighestFitness());
}

TEST(TEST_HNE, theEliteDoesntGetPurgedAfterCenturyEnd)
{
	auto hne = nev::HNE(2, 2, 1, 1);;
	hne.setElitismPercentage(0.5);
	hne.setHighestFitnessAtStartOfCentury(1);
	hne.iterateOneGeneration({1.05, 1.0});
	hne.iterateOneGeneration({1.05, 1.0});
	hne.iterateOneGeneration({1.05, 1.0});

	EXPECT_EQ(1.05, hne.getHighestFitness());
}

TEST(TEST_HNE, theOutputChangesAfterIteration)
{
	auto hne = nev::HNE(10, 1, 1, 1);;
	hne.setMutateWeightProbability(1.0);
	hne.setNewWeightProbability(1.0);
	auto results = hne.getOutputsSnapshot({1.0});
	double previousResult = results[0][0];
	hne.iterateOneGeneration({1.0});
	results = hne.getOutputsSnapshot({1.0});
	double currenResult = results[0][0];

	EXPECT_NE(previousResult, currenResult);
}

#endif