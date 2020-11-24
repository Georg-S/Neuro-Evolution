#include <gtest/gtest.h>
#include <vector>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <FileReader.h>
#include <FileWriter.h>
#include <NEAT.h>

TEST(TEST_FileManagment, SaveToFileAndPopulationGetsParsedCorrectly) {
	nev::NEAT first = nev::NEAT(1, 1, 2, nev::af::relu);
	nev::FileWriter::writeNEATToFile(first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	nev::NEAT second = nev::FileReader::getNEATFromFiles("neat.txt","population.txt", "innovation.txt");
	std::vector<std::vector<double>> secondNetworkOutputs = second.calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++) {
		ASSERT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingPopulationSizeStaysTheSame) {
	nev::NEAT first = nev::NEAT(10, 1, 2);
	nev::FileWriter::writeNEATToFile(first);

	nev::NEAT second = nev::FileReader::getNEATFromFiles("neat.txt", "population.txt", "innovation.txt");

	EXPECT_EQ(first.getPopulationSize(), second.getPopulationSize());
}

TEST(TEST_FileManagment, SaveToFileAndInnovationGetsParsedCorrectly) {
	nev::NEAT first = nev::NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});

	nev::FileWriter::writeNEATToFile(first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	nev::NEAT second = nev::FileReader::getNEATFromFiles("neat.txt", "population.txt", "innovation.txt");
	std::vector<std::vector<double>> secondNetworkOutputs = second.calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++) {
		outputs[i][0] = (int)(outputs[i][0] * 100);
		outputs[i][0] /= 100;
		secondNetworkOutputs[i][0] = (int)(secondNetworkOutputs[i][0] * 100);
		secondNetworkOutputs[i][0] /= 100;
		EXPECT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingInnovationSizeStaysTheSame) {
	nev::NEAT first = nev::NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});
	nev::FileWriter::writeNEATToFile(first);
	nev::NEAT second = nev::FileReader::getNEATFromFiles("neat.txt", "population.txt", "innovation.txt");

	ASSERT_EQ(first.getTotalCountOfInnovations(), second.getTotalCountOfInnovations());
}

TEST(TEST_FileManagment, ParseBetweenQuotationParsesCorrectly) {
	std::string test = "Hallo mein 'Lieber'";
	std::string expected = "Lieber";

	ASSERT_STREQ(expected.c_str(), nev::FileReader::getStringBetweenQuotationMark(test).c_str());
}

TEST(TEST_FileManagment, NEAT_SomeParametersGetSavedAndParsedCorrectly) {
	nev::NEAT neat1 = nev::NEAT(3, 1, 2);
	neat1.setAddNeuronProbability(0.523);
	neat1.setWeightPertubation(0.23);


	nev::FileWriter::writeNEATToFile(neat1);
	nev::NEAT neat2 = nev::FileReader::getNEATFromFiles("neat.txt", "population.txt", "innovation.txt");
	
	ASSERT_DOUBLE_EQ(neat1.getWeightPertubation(), neat2.getWeightPertubation());
	ASSERT_EQ(neat1.getCurrentGeneration(), neat2.getCurrentGeneration());
}
