#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Phenotype.h>
#include <NeuronGene.h>
#include <LinkGene.h>
#include <vector>


TEST(TEST_Genotype, 0by0CreateNoNeurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 0by1CreateNoNeurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 0, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by0CreateNoNeurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 0, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 0);
}

TEST(TEST_Genotype, 1by1Creates3Neurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 3);
}


TEST(TEST_Genotype, 2by2Creates5Neurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 2, 2, 1);

	EXPECT_EQ(geno.getCountOfNeurons(), 5);
}

TEST(TEST_Genotype, 0by0Creates0Links) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 0);
}

TEST(TEST_Genotype, 1by1Creates2Links) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 2);
}

TEST(TEST_Genotype, 2by1Creates3Links) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 2, 1, 1);

	EXPECT_EQ(geno.getCountOfLinks(), 3);
}

TEST(TEST_Genotype, 0by0HasMaxDepthOf0) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 0, 0, 1);

	EXPECT_EQ(geno.getMaxDepth(), 0);
}

TEST(TEST_Genotype, 1by1HasMaxDepthOf1) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfNeurons) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 0);
	int previousNeuronSize = geno.getCountOfNeurons();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousNeuronSize + 1, geno.getCountOfNeurons());
}

TEST(TEST_Genotype, AddNeuronIncrementsSizeOfLinksBy2) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 0);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousLinkSize + 2, geno.getCountOfLinks());
}

TEST(TEST_Genotype, AddNeuronFor1by1NetIncrementsDepthBy1) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousDepth + 1, geno.getMaxDepth());
}

TEST(TEST_Genotype, TwoTimesAddNeuronFor1by1NetIncrementsDepthBy2) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 0);
	int previousDepth = geno.getMaxDepth();
	geno.randomlyAddNeuron(inno, 1.0);
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousDepth + 2, geno.getMaxDepth());
}

TEST(TEST_Genotype, mutateWeightsWithoutNewWeightMutatesWeights) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();
	geno.randomlyMutateAllWeights(1.0, 0.0, 1);
	geno.randomlyMutateAllWeights(1.0, 0.0, 1);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}
TEST(TEST_Genotype, mutateWeightsWithNewWeightMutatesWeights) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);
	double previousLinkWeightAverage = geno.getLinkWeightAverage();
	geno.randomlyMutateAllWeights(1.0, 1.0, 0);

	EXPECT_NE(previousLinkWeightAverage, geno.getLinkWeightAverage());
}

TEST(TEST_Genotype, addLinkAddsNoLinkIfNotPossible) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 2, 1, 1);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousLinkSize, geno.getCountOfLinks());
}

TEST(TEST_Genotype, addLinkAddsALink) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 2, 1, 1);
	geno.randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = geno.getCountOfLinks();
	geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousLinkSize + 1, geno.getCountOfLinks());
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates0ForTheSameGenotype) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	auto geno2 = geno;

	EXPECT_EQ(nev::Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculates2For2InnovationsMore) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	auto geno2 = std::make_shared<nev::Genotype>(*geno.get());
	geno->randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(nev::Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 2);
}

TEST(TEST_Genotype, testSymmetry) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	auto geno2 = geno;
	geno->randomlyAddNeuron(inno, 1.0);

	
	double result1 = nev::Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4);
	double result2 = nev::Genotype::calculateCompatibilityScore(geno2, geno, 1, 1, 0.4);

	EXPECT_EQ(result1, result2);
}

TEST(TEST_Genotype, calculateCompatibilityScoreCalculatesNot0For2DifferentStartingGenotypes) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	std::shared_ptr<nev::Genotype> geno2 = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	
	EXPECT_NE(nev::Genotype::calculateCompatibilityScore(geno, geno2, 1, 1, 0.4), 0);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf1ForNoHiddenNodes) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 1));
	links.push_back(nev::LinkGene(0, 1, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(inno, neurons, links, 1);

	EXPECT_EQ(geno.getMaxDepth(), 1);
}

TEST(TEST_Genotype, theVectorConstructorCalculatesDepthOf2For1HiddenNode) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::NeuronGene> neurons;
	std::vector<nev::LinkGene> links;

	neurons.push_back(nev::NeuronGene(nev::NeuronType::input, 0));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::hidden, 1));
	neurons.push_back(nev::NeuronGene(nev::NeuronType::output, 2));
	links.push_back(nev::LinkGene(0, 1, 1, true, 0, false));
	links.push_back(nev::LinkGene(1, 2, 1, true, 0, false));
	nev::Genotype geno = nev::Genotype(inno, neurons, links, 1);

	EXPECT_EQ(geno.getMaxDepth(), 2);
}

TEST(TEST_Genotype, crossWithItSelfReturnsTheSame) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> father = std::make_shared<nev::Genotype>(inno, 2, 1, 0);

	int previousLinkSize = father->getCountOfLinks();
	int previousNeuronSize = father->getCountOfNeurons();
	double previousLinkAverage = father->getLinkWeightAverage();
	nev::Genotype* baby = nev::Genotype::crossOver(father, father, 1);

	EXPECT_EQ(baby->getCountOfLinks(), previousLinkSize);
	EXPECT_EQ(baby->getCountOfNeurons(), previousNeuronSize);
	EXPECT_EQ(baby->getLinkWeightAverage(), previousLinkAverage);
	delete baby;
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinks) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> father = std::make_shared<nev::Genotype>(inno, 2, 1, 0);
	auto mother = std::make_shared<nev::Genotype>(*(father.get()));
	father->setRawFitness(1.0);
	mother->setRawFitness(1.01);
	mother->randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = mother->getCountOfLinks();

	nev::Genotype* baby = nev::Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby->getCountOfLinks(), previousLinkSize);
	delete baby;
}

TEST(TEST_Genotype, crossOverBabyHasTheRightAmountOfLinksIfFitnessIsDifferent) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> father = std::make_shared<nev::Genotype>(inno, 2, 1, 0);
	auto mother = std::make_shared<nev::Genotype>(*(father.get()));
	father->setRawFitness(1.0);
	mother->setRawFitness(0.9);
	mother->randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = father->getCountOfLinks();

	nev::Genotype* baby = nev::Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby->getCountOfLinks(), previousLinkSize);
	delete baby;
}

TEST(TEST_Genotype, crossOverBabyHasTheRightDepth) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> father = std::make_shared<nev::Genotype>(inno, 2, 1, 0);
	auto mother = std::make_shared<nev::Genotype>(*(father.get()));
	father->setRawFitness(1.0);
	mother->setRawFitness(1.01);
	mother->randomlyAddNeuron(inno, 1.0);
	int previousLinkSize = mother->getCountOfLinks();

	nev::Genotype* baby = nev::Genotype::crossOver(father, mother, 2);

	EXPECT_EQ(baby->getMaxDepth(), 2);
	delete baby;
}