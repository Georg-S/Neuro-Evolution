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
	NEAT first = NEAT(1, 1, 2);
	first.writePopulationAndInnovationAsFiles("population.txt", "innovation.txt");
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	NEAT second = NEAT("population.txt", "innovation.txt");
	std::vector<std::vector<double>> secondNetworkOutputs = second.calculateOutputSnapshot(inputs);

	for (int i = 0; i < outputs.size(); i++) {
		ASSERT_FLOAT_EQ(outputs[i][0], secondNetworkOutputs[i][0]);
	}
}

TEST(TEST_FileManagment, AfterSavingAndParsingPopulationSizeStaysTheSame) {
	NEAT first = NEAT(10, 1, 2);
	first.writePopulationAndInnovationAsFiles("population.txt", "innovation.txt");

	NEAT second = NEAT("population.txt", "innovation.txt");

	EXPECT_EQ(first.getPopulationSize(), second.getPopulationSize());
}

TEST(TEST_FileManagment, SaveToFileAndInnovationGetsParsedCorrectly) {
	NEAT first = NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});

	first.writePopulationAndInnovationAsFiles("population.txt", "innovation.txt");
	std::vector <double> inputs{ 1 };
	std::vector<std::vector<double>> outputs = first.calculateOutputSnapshot(inputs);

	NEAT second = NEAT("population.txt", "innovation.txt");
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
	NEAT first = NEAT(1, 1, 2);
	first.setAddNeuronProbability(1);
	first.iterateOneGeneration(std::vector<double>{0, 1});

	first.writePopulationAndInnovationAsFiles("population.txt", "innovation.txt");
	NEAT second = NEAT("population.txt", "innovation.txt");

	ASSERT_EQ(first.getTotalCountOfInnovations(), second.getTotalCountOfInnovations());
}

TEST(TEST_FileManagment, ParseBetweenQuotationParsesCorrectly) {
	FileReader reader = FileReader();
	std::string test = "Hallo mein 'Lieber'";
	std::string expected = "Lieber";

	ASSERT_STREQ(expected.c_str(), reader.getStringBetweenQuotationMark(test).c_str());
}
