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

nev::Genotype createXORGenotype() {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 2));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 3));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::hidden, 4));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::hidden, 5));

	std::vector<nev::LinkGene> links;
	links.push_back(nev::LinkGene(1, 4, 20, 1, true));
	links.push_back(nev::LinkGene(2, 4, 20, 1, true));
	links.push_back(nev::LinkGene(0, 4, -10, 1, true));

	links.push_back(nev::LinkGene(1, 5, -20, 1, true));
	links.push_back(nev::LinkGene(2, 5, -20, 1, true));
	links.push_back(nev::LinkGene(0, 5, 30, 1, true));

	links.push_back(nev::LinkGene(4, 3, 20, 1, true));
	links.push_back(nev::LinkGene(5, 3, 20, 1, true));
	links.push_back(nev::LinkGene(0, 3, -30, 1, true));

	return nev::Genotype(&inno, neurons, links, 1);
}

TEST(TEST_XOR, XORIsPossibleFirstInput) {
	nev::Genotype geno = createXORGenotype();
	geno.createPhenotype();
	std::vector<double> output = geno.phenotype->calculateOutputSnapshot(std::vector<double>{0.0, 0.0}, nev::af::steepenedSigmoid);

	EXPECT_LT(output[0], 0.1);
}

TEST(TEST_XOR, XORIsPossibleSecondInput) {
	nev::Genotype geno = createXORGenotype();
	geno.createPhenotype();
	std::vector<double> output = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0, 0.0}, nev::af::steepenedSigmoid);

	EXPECT_GT(output[0], 0.9);
}
TEST(TEST_XOR, XORIsPossibleThirdInput) {
	nev::Genotype geno = createXORGenotype();
	geno.createPhenotype();
	std::vector<double> output = geno.phenotype->calculateOutputSnapshot(std::vector<double>{0.0, 1.0}, nev::af::steepenedSigmoid);

	EXPECT_GT(output[0], 0.9);
}
TEST(TEST_XOR, XORIsPossibleFourthInput) {
	nev::Genotype geno = createXORGenotype();
	geno.createPhenotype();
	std::vector<double> output = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0, 1.0}, nev::af::steepenedSigmoid);

	EXPECT_LT(output[0], 0.1);
}

std::vector<double> calculateFitness(nev::NEAT& neat) {
	std::vector<double> fitness;

	std::vector<std::vector<double>> outputs = neat.calculateOutputSnapshot(std::vector<double> {0, 0});
	for (int i = 0; i < outputs.size(); i++) {
		fitness.push_back(1 - outputs[i][0]);
	}

	outputs = neat.calculateOutputSnapshot(std::vector<double> {1, 1});
	for (int i = 0; i < outputs.size(); i++) {
		fitness[i] += (1 - outputs[i][0]);
	}


	outputs = neat.calculateOutputSnapshot(std::vector<double> {1, 0});
	for (int i = 0; i < outputs.size(); i++) {
		fitness[i] += outputs[i][0];
	}

	outputs = neat.calculateOutputSnapshot(std::vector<double> {0, 1});
	for (int i = 0; i < outputs.size(); i++) {
		fitness[i] += outputs[i][0];
	}


	return fitness;
}

double getHighestFitness(const std::vector<double> fitness) {
	double highestFitness = 0;
	for (int i = 0; i < fitness.size(); i++) {
		if (fitness[i] > highestFitness)
			highestFitness = fitness[i];
	}
	return highestFitness;
}

void testXOR() {
	double highestFitnessEver = 0;
	std::vector<double> fitness;
	nev::NEAT neat = nev::NEAT(150, 2, 1);

	for (int i = 0; i < INT_MAX; i++) {
		fitness.clear();
		fitness = calculateFitness(neat);
		double currentHighestFitness = getHighestFitness(fitness);
		if (currentHighestFitness > highestFitnessEver) {
			highestFitnessEver = currentHighestFitness;
		}
		if (highestFitnessEver > 3) {
			std::cout << "Solution Found" << std::endl;
		}
		std::cout << i << std::endl;
		std::cout << currentHighestFitness << std::endl;
		neat.iterateOneGeneration(fitness);
		std::cout << std::endl;
	}
}


void testXorParameters() {
	std::vector<double> fitness;
	double generationAverage = 0;
	bool solutionFound = false;

	for (int x = 0; x < 100; x++) {
		nev::NEAT neat = nev::NEAT(150, 2, 1);
		solutionFound = false;
		double highestFitnessEver = 0;

		while (!solutionFound) {
			fitness.clear();
			fitness = calculateFitness(neat);
			double currentHighestFitness = getHighestFitness(fitness);
			if (currentHighestFitness > highestFitnessEver) {
				highestFitnessEver = currentHighestFitness;
			}
			if (highestFitnessEver > 3) {
				generationAverage += neat.getCurrentGeneration();
				solutionFound = true;
				std::cout << "Solution Found" << std::endl;
			}
			else
				neat.iterateOneGeneration(fitness);
		}
	}
	generationAverage /= 100;

	std::cout << "Average is " << generationAverage << std::endl;
	getchar();
}

#endif

/*
TEST(TEST_XOR, MemoryLeak) {
	while (true) {
		nev::NEAT neat = nev::NEAT(150, 2, 1);
		std::vector<std::vector<double>> outputs = neat.calculateOutputSnapshot(std::vector<double> {0, 0});
	}
}
*/

/*
TEST(TEST_XOR, NEATXorTest) {
	testXOR();
}
*/

/*
TEST(TEST_XOR, NEATXorPracticeTest) {
	testXorParameters();
}
*/