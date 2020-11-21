#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <vector>

TEST(TEST_Genotype, 0by0CreateNoNeurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 0by1CreateNoNeurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 0, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by0CreateNoNeurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by1Creates3Neurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 3);
}


TEST(TEST_Genotype, 2by2Creates5Neurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 2, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 5);
}

TEST(TEST_Genotype, 0by0Creates0Links) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 0);
}

TEST(TEST_Genotype, 1by1Creates2Links) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 2);
}

TEST(TEST_Genotype, 2by1Creates3Links) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 3);
}

TEST(TEST_Genotype, 0by0HasMaxDepthOf0) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getMaxDepth(), 0);
}

TEST(TEST_Genotype, 1by1HasMaxDepthOf1) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfNeurons) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 0);
	int previousNeuronSize = geno.getCountOfNeurons();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousNeuronSize + 1, geno.getCountOfNeurons());
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfLinksBy2) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 0);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousLinkSize + 2, geno.getCountOfLinks());
}

TEST(TEST_Genotype, AddNeuronFor1by1NetIncrementsDepthBy1) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousDepth + 1, geno.getMaxDepth());
}

TEST(TEST_Genotype, TwoTimesAddNeuronFor1by1NetIncrementsDepthBy2) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(inno, 1.0);
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousDepth + 2, geno.getMaxDepth());
}

TEST(TEST_Genotype, mutateWeightsWithoutNewWeightMutatesWeights) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();
	geno.randomlyMutateAllWeights(1.0, 0.0, 1);
	geno.randomlyMutateAllWeights(1.0, 0.0, 1);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}
TEST(TEST_Genotype, mutateWeightsWithNewWeightMutatesWeights) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();
	geno.randomlyMutateAllWeights(1.0, 1.0, 0);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}

TEST(TEST_Genotype, addLinkAddsNoLinkIfNotPossible) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousLinkSize, geno.getCountOfLinks());
}

TEST(TEST_Genotype, addLinkAddsALink) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);
	geno.randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousLinkSize + 1, geno.getCountOfLinks());
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates0ForTheSameGenotype) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = geno;

	EXPECT_EQ(Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates2For2InnovationsMore) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = geno;
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 2);
}

TEST(TEST_Genotype, testSymmetry) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = geno;
	geno.randomlyAddNeuron(inno, 1.0);

	
	double result1 = Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4);
	double result2 = Genotype::calculateCompatibilityScore(geno2, geno, 1, 1, 0.4);

	EXPECT_EQ(result1, result2);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculatesNot0For2DifferentStartingGenotypes) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = Genotype(inno, 1, 1, 1);

	
	EXPECT_NE(Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf1ForNoHiddenNodes) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::output, 1));
	links.push_back(LinkGene(0, 1, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf2For1HiddenNode) {
	Innovation inno = Innovation();
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	neurons.push_back(NeuronGene(NeuronType::input, 0));
	neurons.push_back(NeuronGene(NeuronType::hidden, 1));
	neurons.push_back(NeuronGene(NeuronType::output, 2));
	links.push_back(LinkGene(0, 1, 1, true, 0, false));
	links.push_back(LinkGene(1, 2, 1, true, 0, false));
	Genotype geno = Genotype(inno, neurons, links, 1);

	EXPECT_EQ(geno.getMaxDepth(), 2);
}

TEST(TEST_Genotype, crossWithItSelfReturnsTheSame) {
	Innovation inno = Innovation();
	Genotype father = Genotype(inno, 2, 1, 0);
	int previousLinkSize = father.getCountOfLinks();
	int previousNeuronSize = father.getCountOfNeurons();
	double previousLinkAverage = father.getLinkWeightAverage();
	Genotype baby = Genotype::crossOver(father, father, 1);

	EXPECT_EQ(baby.getCountOfLinks(), previousLinkSize);
	EXPECT_EQ(baby.getCountOfNeurons(), previousNeuronSize);
	EXPECT_EQ(baby.getLinkWeightAverage(), previousLinkAverage);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinks) {
	Innovation inno = Innovation();
	Genotype father = Genotype(inno, 2, 1, 0);
	Genotype mother = father;
	father.setRawFitness(1.0);
	mother.setRawFitness(1.01);
	mother.randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = mother.getCountOfLinks();

	Genotype baby = Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby.getCountOfLinks(), previousLinkSize);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinksIfFitnessIsDifferent) {
	Innovation inno = Innovation();
	Genotype father = Genotype(inno, 2, 1, 0);
	Genotype mother = father;
	father.setRawFitness(1.0);
	mother.setRawFitness(0.9);
	mother.randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = father.getCountOfLinks();

	Genotype baby = Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby.getCountOfLinks(), previousLinkSize);
}

TEST(TEST_Genotype, crossOverBabyHasTheRightDepth) {
	Innovation inno = Innovation();
	Genotype father = Genotype(inno, 2, 1, 0);
	Genotype mother = father;
	father.setRawFitness(1.0);
	mother.setRawFitness(1.01);
	mother.randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = mother.getCountOfLinks();

	Genotype baby = Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby.getMaxDepth(), 2);
}