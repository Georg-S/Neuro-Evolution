#include <gtest/gtest.h>
#include <NEAT.h>
#include <vector>

TEST(TEST_NEAT, constructorCreatesRightAmountOfGenotypes) {
	NEAT neat = NEAT(50, 1, 1);
	EXPECT_EQ(neat.getPopulationSize(), 50);
}

TEST(TEST_NEAT, afterConstructionReturnsTheRightHighestGenotypeID) {
	NEAT neat = NEAT(50, 1, 1);
	EXPECT_EQ(neat.getHighestGenotypeId(), 49);
}

TEST(TEST_NEAT, afterConstructionA1By1Net2InnovationsAreCreated) {
	NEAT neat = NEAT(50, 1, 1);
	EXPECT_EQ(neat.getTotalCountOfInnovations(), 2);
}

TEST(TEST_NEAT, calculateOutputSnapshotReturnsRightAmountOfOutputs) {
	NEAT neat = NEAT(50, 1, 1);
	std::vector<std::vector<double>> outputs = neat.calculateOutputSnapshot(std::vector<double>{1.0});
	EXPECT_EQ(outputs.size(), 50);
}

TEST(TEST_NEAT, afterOneIterationNoSpeciesIsCreatedIfNotEnoughFitnessValuesArePassed) {
	NEAT neat = NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 10; i++)
		fitness.push_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getSpeciesCount(), 0);
}

TEST(TEST_NEAT, afterOneIterationASpeciesIsCreated) {
	NEAT neat = NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.push_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getSpeciesCount(), 1);
}

TEST(TEST_NEAT, afterOneIterationPopulationSizeIsTheSame) {
	NEAT neat = NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.push_back(1.0);
	neat.iterateOneGeneration(fitness);

	EXPECT_EQ(neat.getPopulationSize(), 50);
}

TEST(TEST_NEAT, after10IterationPopulationSizeIsTheSame) {
	NEAT neat = NEAT(50, 1, 1);
	std::vector<double> fitness;
	for (int i = 0; i < 50; i++)
		fitness.push_back(1.0);

	for(int i = 0; i < 10; i++)
		neat.iterateOneGeneration(fitness);	

	EXPECT_EQ(neat.getPopulationSize(), 50);
}