#include <gtest/gtest.h>
#include <NEAT.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_NEAT, constructorCreatesRightAmountOfGenotypes)
{
	auto neat = nev::NEAT(50, 1, 1);

	EXPECT_EQ(neat.getPopulationSize(), 50);
}

TEST(TEST_NEAT, afterConstructionReturnsTheRightHighestGenotypeID)
{
	auto neat = nev::NEAT(50, 1, 1);

	EXPECT_EQ(neat.getHighestGenotypeId(), 49);
}

TEST(TEST_NEAT, afterConstructionA1By1Net2InnovationsAreCreated)
{
	auto neat = nev::NEAT(50, 1, 1);

	EXPECT_EQ(neat.getTotalCountOfInnovations(), 2);
}

TEST(TEST_NEAT, calculateOutputSnapshotReturnsRightAmountOfOutputs)
{
	auto neat = nev::NEAT(50, 1, 1);
	auto outputs = neat.calculateOutputSnapshot(std::vector<double>{1.0});

	EXPECT_EQ(outputs.size(), 50);
}

TEST(TEST_NEAT, afterOneIterationNoSpeciesIsCreatedIfNotEnoughFitnessValuesArePassed)
{
	auto neat = nev::NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 10; i++)
		fitness.emplace_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getSpeciesCount(), 0);
}

TEST(TEST_NEAT, afterOneIterationASpeciesIsCreated)
{
	auto neat = nev::NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.emplace_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getSpeciesCount(), 1);
}

TEST(TEST_NEAT, afterOneIterationPopulationSizeIsTheSame)
{
	auto neat = nev::NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.emplace_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getPopulationSize(), 50);
}

TEST(TEST_NEAT, after10IterationPopulationSizeIsTheSame)
{
	auto neat = nev::NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.emplace_back(1.0);

	for (int i = 0; i < 10; i++)
		neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getPopulationSize(), 50);
}

#endif