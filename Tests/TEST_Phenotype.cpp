#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <vector>


TEST(TEST_Phenotype, PhenotypeWithoutLinksToOutputReturnsNoOutputCauseMaxDepthIs0) {
	Innovation inno = Innovation();
	std::vector<LinkGene> links;
	std::vector<NeuronGene> neurons;
	neurons.push_back(NeuronGene(NeuronType::bias, 0));
	neurons.push_back(NeuronGene(NeuronType::input, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	Genotype geno = Genotype(inno,neurons, links,1);
	geno.createPhenotype();

	std::vector<double> outputs = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0});
	EXPECT_DOUBLE_EQ(outputs.size(), 0);
}
TEST(TEST_Phenotype, Simple1By1GenotypeWithoutBiasPhenotypeHasAnOutput) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	links.push_back(LinkGene(0, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);
	geno.createPhenotype();
	std::vector<double> outputs = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0});

	EXPECT_EQ(outputs.size(), 1);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith0AsInputReturnsCorrectValue) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	links.push_back(LinkGene(0, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);
	geno.createPhenotype();
	std::vector<double> outputs = geno.phenotype->calculateOutputSnapshot(std::vector<double>{0.0});

	EXPECT_EQ(outputs[0], 0.5);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1AsInputReturnsCorrectValueWithSteepenedSigmoid) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	links.push_back(LinkGene(0, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);
	geno.createPhenotype();
	std::vector<double> outputs = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0});

	EXPECT_DOUBLE_EQ(outputs[0], 0.99260845865571812);
}

TEST(TEST_Phenotype, PhenotypeAfterAddNeuronHasAlmostSameOutputWithSteepenedSigmoid) {
	Innovation inno = Innovation();
	std::vector<LinkGene> links;
	std::vector<NeuronGene> neurons;
	neurons.push_back(NeuronGene(NeuronType::bias, 0));
	neurons.push_back(NeuronGene(NeuronType::input, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	links.push_back(LinkGene(1, 2, 1, true,0));
	Genotype geno = Genotype(inno,neurons, links,0);
	geno.randomlyAddNeuron(inno,1.0);
	geno.createPhenotype();


	std::vector<double> outputs = geno.phenotype->calculateOutputSnapshot(std::vector<double>{1.0});
	EXPECT_DOUBLE_EQ(outputs[0], 0.99233792943072541);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1HiddenReturn0_5ForActiveOneTime) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	neurons.push_back(NeuronGene(NeuronType::hidden, 2));
	links.push_back(LinkGene(0, 2, 1, true, 0, false));
	links.push_back(LinkGene(2, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);
	geno.createPhenotype();
	std::vector<double> outputs = geno.phenotype->calculateOutputActive(std::vector<double>{1.0});

	EXPECT_EQ(outputs[0], 0.5);
}

TEST(TEST_Phenotype, Simple1By1GenotypeWith1HiddenReturn0_5ForActiveTwoTimes) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	neurons.push_back(NeuronGene(NeuronType::hidden, 2));
	links.push_back(LinkGene(0, 2, 1, true, 0, false));
	links.push_back(LinkGene(2, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);
	geno.createPhenotype();
	std::vector<double> outputs = geno.phenotype->calculateOutputActive(std::vector<double>{1.0});
	outputs = geno.phenotype->calculateOutputActive(std::vector<double>{1.0});

	ASSERT_FLOAT_EQ(outputs[0], 0.992338);
}