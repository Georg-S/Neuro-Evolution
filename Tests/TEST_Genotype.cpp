#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_Genotype, 0by0CreateNoNeurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 0by1CreateNoNeurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 0, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by0CreateNoNeurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by1Creates3Neurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 3);
}


TEST(TEST_Genotype, 2by2Creates5Neurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 2, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 5);
}

TEST(TEST_Genotype, 0by0Creates0Links)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 0);
}

TEST(TEST_Genotype, 1by1Creates2Links)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 2);
}

TEST(TEST_Genotype, 2by1Creates3Links)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 3);
}

TEST(TEST_Genotype, 0by0HasMaxDepthOf0)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 0, 0, 1);

	EXPECT_EQ(geno.getMaxDepth(), 0);
}

TEST(TEST_Genotype, 1by1HasMaxDepthOf1)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfNeurons)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 0);
	int previousNeuronSize = geno.getCountOfNeurons();
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousNeuronSize + 1, geno.getCountOfNeurons());
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfLinksBy2)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 0);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousLinkSize + 2, geno.getCountOfLinks());
}

TEST(TEST_Genotype, AddNeuronFor1by1NetIncrementsDepthBy1)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousDepth + 1, geno.getMaxDepth());
}

TEST(TEST_Genotype, TwoTimesAddNeuronFor1by1NetIncrementsDepthBy2)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(&inno, 1.0);
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousDepth + 2, geno.getMaxDepth());
}

TEST(TEST_Genotype, mutateWeightsWithoutNewWeightMutatesWeights)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();

	// This test was (maybe still is) not deterministic, therefore we execute it multiple time
	// So the odds that the test fails are very small
	for (int i = 0; i < 20; i++)
		geno.randomlyMutateAllWeights(1.0, 0.0, 0.1 * i);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}

TEST(TEST_Genotype, mutateWeightsWithNewWeightMutatesWeights)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();
	geno.randomlyMutateAllWeights(1.0, 1.0, 0);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}

TEST(TEST_Genotype, addLinkAddsNoLinkIfNotPossible)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddLink(&inno, 1.0, false);

	EXPECT_EQ(previousLinkSize, geno.getCountOfLinks());
}

TEST(TEST_Genotype, addLinkAddsALink)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);
	geno.randomlyAddNeuron(&inno, 1.0);
	int previousLinkSize = geno.getCountOfLinks();
	// If very unlucky this Test can be not deterministic
	geno.randomlyAddLink(&inno, 1.0, true);

	EXPECT_EQ(previousLinkSize + 1, geno.getCountOfLinks());
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates0ForTheSameGenotype)
{
	auto inno = nev::Innovation();
	auto geno = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto geno2 = std::make_unique<nev::Genotype>(*geno);

	EXPECT_EQ(nev::Genotype::calculateCompatibilityScore(geno.get(), geno2.get(), 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates2For2InnovationsMore)
{
	auto inno = nev::Innovation();
	auto geno = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto geno2 = std::make_unique<nev::Genotype>(*geno);
	geno->randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(nev::Genotype::calculateCompatibilityScore(geno.get(), geno2.get(), 1, 1, 0.4), 2);
}

TEST(TEST_Genotype, testSymmetry)
{
	auto inno = nev::Innovation();
	auto geno = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto geno2 = std::make_unique<nev::Genotype>(*geno);
	geno->randomlyAddNeuron(&inno, 1.0);

	double result1 = nev::Genotype::calculateCompatibilityScore(geno.get(), geno2.get(), 1, 1, 0.4);
	double result2 = nev::Genotype::calculateCompatibilityScore(geno2.get(), geno.get(), 1, 1, 0.4);

	EXPECT_EQ(result1, result2);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculatesNot0For2DifferentStartingGenotypes)
{
	auto inno = nev::Innovation();
	auto geno = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto geno2 = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);

	EXPECT_NE(nev::Genotype::calculateCompatibilityScore(geno.get(), geno2.get(), 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf1ForNoHiddenNodes)
{
	auto inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.emplace_back(nev::NeuronType::input, 0);
	neurons.emplace_back(nev::NeuronType::output, 1);
	links.emplace_back(0, 1, 1, true, 0, false);
	auto geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf2For1HiddenNode)
{
	auto inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.emplace_back(nev::NeuronType::input, 0);
	neurons.emplace_back(nev::NeuronType::hidden, 1);
	neurons.emplace_back(nev::NeuronType::output, 2);
	links.emplace_back(0, 1, 1, true, 0, false);
	links.emplace_back(1, 2, 1, true, 0, false);
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);

	EXPECT_EQ(geno.getMaxDepth(), 2);
}

TEST(TEST_Genotype, crossWithItSelfReturnsTheSame)
{
	auto inno = nev::Innovation();
	auto father = std::make_unique<nev::Genotype>(&inno, 2, 1, 0);

	int previousLinkSize = father->getCountOfLinks();
	int previousNeuronSize = father->getCountOfNeurons();
	double previousLinkAverage = father->getLinkWeightAverage();
	auto baby = nev::Genotype::crossOver(father.get(), father.get(), 1);

	EXPECT_EQ(baby->getCountOfLinks(), previousLinkSize);
	EXPECT_EQ(baby->getCountOfNeurons(), previousNeuronSize);
	EXPECT_EQ(baby->getLinkWeightAverage(), previousLinkAverage);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinks)
{
	auto inno = nev::Innovation();
	auto father = std::make_unique<nev::Genotype>(&inno, 2, 1, 0);
	auto mother = std::make_unique<nev::Genotype>(*father);
	father->setRawFitness(1.0);
	mother->setRawFitness(1.01);
	mother->randomlyAddNeuron(&inno, 1.0);
	int motherLinkSize = mother->getCountOfLinks();

	auto baby = nev::Genotype::crossOver(father.get(), mother.get(), 2);

	EXPECT_EQ(baby->getCountOfLinks(), motherLinkSize);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinksIfFitnessIsDifferent)
{
	auto inno = nev::Innovation();
	auto father = std::make_unique<nev::Genotype>(&inno, 2, 1, 0);
	auto mother = std::make_unique<nev::Genotype>(*father);
	father->setRawFitness(1.0);
	mother->setRawFitness(0.9);
	mother->randomlyAddNeuron(&inno, 1.0);
	int fatherLinkSize = father->getCountOfLinks();

	auto baby = nev::Genotype::crossOver(father.get(), mother.get(), 2);

	EXPECT_EQ(baby->getCountOfLinks(), fatherLinkSize);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightDepth)
{
	auto inno = nev::Innovation();
	auto father = std::make_unique<nev::Genotype>(&inno, 2, 1, 0);
	auto mother = std::make_unique<nev::Genotype>(*father);
	father->setRawFitness(1.0);
	mother->setRawFitness(1.01);
	mother->randomlyAddNeuron(&inno, 1.0);
	int previousLinkSize = mother->getCountOfLinks();

	auto baby = nev::Genotype::crossOver(father.get(), mother.get(), 2);

	EXPECT_EQ(baby->getMaxDepth(), 2);
}

#endif