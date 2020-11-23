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
	nev::NEAT first = nev::NEAT(1, 1, 2);
	nev::FileWriter::writeNEATToFile(first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	nev::NEAT second = nev::NEAT("population.txt", "innovation.txt");
	std::vector<std::vector<double>> secondNetworkOutputs = second.calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++) {
		ASSERT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingPopulationSizeStaysTheSame) {
	nev::NEAT first = nev::NEAT(10, 1, 2);
	nev::FileWriter::writeNEATToFile(first);

	nev::NEAT second = nev::NEAT("population.txt", "innovation.txt");

	EXPECT_EQ(first.getPopulationSize(), second.getPopulationSize());
}

TEST(TEST_FileManagment, SaveToFileAndInnovationGetsParsedCorrectly) {
	nev::NEAT first = nev::NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});

	nev::FileWriter::writeNEATToFile(first);
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	nev::NEAT second = nev::NEAT("population.txt", "innovation.txt");
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
	nev::NEAT second = nev::NEAT("population.txt", "innovation.txt");

	ASSERT_EQ(first.getTotalCountOfInnovations(), second.getTotalCountOfInnovations());
}

TEST(TEST_FileManagment, ParseBetweenQuotationParsesCorrectly) {
	nev::FileReader reader = nev::FileReader();
	std::string test = "Hallo mein 'Lieber'";
	std::string expected = "Lieber";

	ASSERT_STREQ(expected.c_str(), reader.getStringBetweenQuotationMark(test).c_str());
}
