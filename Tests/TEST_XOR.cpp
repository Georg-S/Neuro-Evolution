#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <NEAT.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

static nev::Genotype createXORGenotype()
{
	auto inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	neurons.emplace_back(nev::NeuronType::bias, 0);
	neurons.emplace_back(nev::NeuronType::input, 1);
	neurons.emplace_back(nev::NeuronType::input, 2);
	neurons.emplace_back(nev::NeuronType::output, 3);
	neurons.emplace_back(nev::NeuronType::hidden, 4);
	neurons.emplace_back(nev::NeuronType::hidden, 5);

	std::vector<nev::LinkGene> links;
	links.emplace_back(1, 4, 20, 1, true);
	links.emplace_back(2, 4, 20, 1, true);
	links.emplace_back(0, 4, -10, 1, true);

	links.emplace_back(1, 5, -20, 1, true);
	links.emplace_back(2, 5, -20, 1, true);
	links.emplace_back(0, 5, 30, 1, true);

	links.emplace_back(4, 3, 20, 1, true);
	links.emplace_back(5, 3, 20, 1, true);
	links.emplace_back(0, 3, -30, 1, true);

	return nev::Genotype(&inno, std::move(neurons), std::move(links), 1, nev::af::steepenedSigmoid);
}

TEST(TEST_XOR, XORIsPossibleFirstInput)
{
	auto geno = createXORGenotype();
	geno.createPhenotype();
	auto output = geno.calculateOutputSnapshot(std::vector<double>{0.0, 0.0});

	EXPECT_LT(output[0], 0.1);
}

TEST(TEST_XOR, XORIsPossibleSecondInput)
{
	auto geno = createXORGenotype();
	geno.createPhenotype();
	auto output = geno.calculateOutputSnapshot(std::vector<double>{1.0, 0.0});

	EXPECT_GT(output[0], 0.9);
}

TEST(TEST_XOR, XORIsPossibleThirdInput)
{
	auto geno = createXORGenotype();
	geno.createPhenotype();
	auto output = geno.calculateOutputSnapshot(std::vector<double>{0.0, 1.0});

	EXPECT_GT(output[0], 0.9);
}

TEST(TEST_XOR, XORIsPossibleFourthInput)
{
	auto geno = createXORGenotype();
	geno.createPhenotype();
	auto output = geno.calculateOutputSnapshot(std::vector<double>{1.0, 1.0});

	EXPECT_LT(output[0], 0.1);
}

#endif

static std::vector<double> calculateFitness(nev::NEAT& neat)
{
	std::vector<double> fitness;

	auto outputs = neat.calculateOutputSnapshot(std::vector<double> {0, 0});
	for (const auto& output : outputs)
		fitness.push_back(1 - output.front());

	outputs = neat.calculateOutputSnapshot(std::vector<double> {1, 1});
	for (int i = 0; i < outputs.size(); i++)
		fitness[i] += (1 - outputs[i].front());

	outputs = neat.calculateOutputSnapshot(std::vector<double> {1, 0});
	for (int i = 0; i < outputs.size(); i++)
		fitness[i] += outputs[i][0];

	outputs = neat.calculateOutputSnapshot(std::vector<double> {0, 1});
	for (int i = 0; i < outputs.size(); i++)
		fitness[i] += outputs[i][0];

	return fitness;
}

static double getHighestFitness(const std::vector<double> fitness)
{
	double highestFitness = 0;
	for (auto fit : fitness)
		highestFitness = std::max(highestFitness, fit);

	return highestFitness;
}

static bool testXOR(bool runIndefinite = false)
{
	double highestFitnessEver = 0;
	std::vector<double> fitness;
	auto neat = nev::NEAT(150, 2, 1);
	int i = 0;

	while (true)
	{
		fitness.clear();
		fitness = calculateFitness(neat);
		double currentHighestFitness = getHighestFitness(fitness);
		highestFitnessEver = std::max(currentHighestFitness, highestFitnessEver);

		if (highestFitnessEver > 3)
		{
			if (runIndefinite)
				std::cout << "Solution Found" << std::endl;
			else
				return true;
		}

		if (runIndefinite)
		{
			std::cout << i++ << std::endl;
			std::cout << currentHighestFitness << std::endl;
			std::cout << std::endl;
		}

		neat.iterateOneGeneration(fitness);
	}
}


static void testXorParameters()
{
	std::vector<double> fitness;
	double generationAverage = 0;
	bool solutionFound = false;

	for (int x = 0; x < 100; x++)
	{
		auto neat = nev::NEAT(150, 2, 1);
		solutionFound = false;
		double highestFitnessEver = 0;

		while (!solutionFound)
		{
			fitness.clear();
			fitness = calculateFitness(neat);
			double currentHighestFitness = getHighestFitness(fitness);
			highestFitnessEver = std::max(currentHighestFitness, highestFitnessEver);

			if (highestFitnessEver > 3)
			{
				generationAverage += neat.getCurrentGeneration();
				solutionFound = true;
				std::cout << "Solution Found" << std::endl;
			}
			else
			{
				neat.iterateOneGeneration(fitness);
			}
		}
	}
	generationAverage /= 100;

	std::cout << "Average is " << generationAverage << std::endl;
	getchar();
}


#if RUN_LONG_TESTS

TEST(TEST_XOR, NEATXorTest)
{
	testXOR();

	ASSERT_EQ(1, 1);
}

#endif

/*
TEST(TEST_XOR, NEATXorPracticeTest) {
	testXorParameters();
}
*/