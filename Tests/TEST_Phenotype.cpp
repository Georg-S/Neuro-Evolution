#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_Phenotype, PhenotypeWithoutLinksToOutputReturnsNoOutputCauseMaxDepthIs0) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links),1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();

	std::vector<double> outputs = geno.calculateOutputSnapshot(std::vector<double>{1.0});
	EXPECT_DOUBLE_EQ(outputs.size(), 0);
}
TEST(TEST_Phenotype, Simple1By1GenotypeWithoutBiasPhenotypeHasAnOutput) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	links.push_back(nev::LinkGene(0, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();
	std::vector<double> outputs = geno.calculateOutputSnapshot(std::vector<double>{1.0});

	EXPECT_EQ(outputs.size(), 1);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith0AsInputReturnsCorrectValue) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	links.push_back(nev::LinkGene(0, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();
	std::vector<double> outputs = geno.calculateOutputSnapshot(std::vector<double>{0.0});

	EXPECT_EQ(outputs[0], 0.5);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1AsInputReturnsCorrectValueWithSteepenedSigmoid) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	links.push_back(nev::LinkGene(0, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();
	std::vector<double> outputs = geno.calculateOutputSnapshot(std::vector<double>{1.0});

	EXPECT_DOUBLE_EQ(outputs[0], 0.99260845865571812);
}

TEST(TEST_Phenotype, PhenotypeAfterAddNeuronHasAlmostSameOutputWithSteepenedSigmoid) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::LinkGene> links;
	std::vector<nev::NeuronGene> neurons;
	neurons.push_back(nev::NeuronGene(nev::NeuronType::bias, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	links.push_back(nev::LinkGene(1, 2, 1, true,0));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links),0);
	geno.randomlyAddNeuron(&inno,1.0);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();

	std::vector<double> outputs = geno.calculateOutputSnapshot(std::vector<double>{1.0});
	EXPECT_DOUBLE_EQ(outputs[0], 0.99233792943072541);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1HiddenReturn0_5ForActiveOneTime) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::hidden, 2));
	links.push_back(nev::LinkGene(0, 2, 1, true, 0, false));
	links.push_back(nev::LinkGene(2, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();
	std::vector<double> outputs = geno.calculateOutputActive(std::vector<double>{1.0});

	EXPECT_EQ(outputs[0], 0.5);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1HiddenReturn0_5ForActiveTwoTimes) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::hidden, 2));
	links.push_back(nev::LinkGene(0, 2, 1, true, 0, false));
	links.push_back(nev::LinkGene(2, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(&inno, std::move(neurons), std::move(links), 1);
	geno.setActivationFunction(nev::af::steepenedSigmoid);
	geno.createPhenotype();
	std::vector<double> outputs = geno.calculateOutputActive(std::vector<double>{1.0});
	outputs = geno.calculateOutputActive(std::vector<double>{1.0});

	ASSERT_FLOAT_EQ(outputs[0], 0.992338);
}

#endif