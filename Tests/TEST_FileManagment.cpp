#include <gtest/gtest.h>
#include <vector>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <NEAT.h>
#include <NEATFileIO.h>
#include "Test_config.h"

static std::string innovationPath = "neat_innovations.json";
static std::string populationPath = "neat_population.json";
static std::string parametersPath = "neat_parameters.json";

#if RUN_NORMAL_TESTS

TEST(TEST_FileManagment, SaveToFileAndPopulationGetsParsedCorrectly)
{
	auto first = nev::NEAT(1, 1, 2, nev::af::relu);
	nev::FileIO::saveToFile("", first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	auto second = nev::FileIO::loadNEATFromFiles(parametersPath, populationPath, innovationPath);
	std::vector<std::vector<double>> secondNetworkOutputs = second->calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++)
	{
		ASSERT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingPopulationSizeStaysTheSame)
{
	auto first = nev::NEAT(10, 1, 2);
	nev::FileIO::saveToFile("", first);

	auto second = nev::FileIO::loadNEATFromFiles(parametersPath, populationPath, innovationPath);

	EXPECT_EQ(first.getPopulationSize(), second->getPopulationSize());
}

TEST(TEST_FileManagment, SaveToFileAndInnovationGetsParsedCorrectly)
{
	auto first = nev::NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});

	nev::FileIO::saveToFile("", first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	auto second = nev::FileIO::loadNEATFromFiles(parametersPath, populationPath, innovationPath);
	std::vector<std::vector<double>> secondNetworkOutputs = second->calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++)
	{
		outputs[i][0] = (int)(outputs[i][0] * 100);
		outputs[i][0] /= 100;
		secondNetworkOutputs[i][0] = (int)(secondNetworkOutputs[i][0] * 100);
		secondNetworkOutputs[i][0] /= 100;
		EXPECT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingInnovationSizeStaysTheSame)
{
	auto first = nev::NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});
	nev::FileIO::saveToFile("", first);
	auto second = nev::FileIO::loadNEATFromFiles(parametersPath, populationPath, innovationPath);

	ASSERT_EQ(first.getTotalCountOfInnovations(), second->getTotalCountOfInnovations());
}

TEST(TEST_FileManagment, NEAT_SomeParametersGetSavedAndParsedCorrectly)
{
	auto neat1 = nev::NEAT(3, 1, 2);
	neat1.setAddNeuronProbability(0.523);
	neat1.setWeightPertubation(0.23);

	nev::FileIO::saveToFile("", neat1);
	auto neat2 = nev::FileIO::loadNEATFromFiles(parametersPath, populationPath, innovationPath);

	ASSERT_DOUBLE_EQ(neat1.getWeightPertubation(), neat2->getWeightPertubation());
	ASSERT_EQ(neat1.getCurrentGeneration(), neat2->getCurrentGeneration());
}

#endif