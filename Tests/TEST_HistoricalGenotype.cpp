#include <gtest/gtest.h>
#include <HNE/HNE.h>


TEST(TEST_HistoricalGenotype, HistorySizeIs1AfterCreation) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno, 1, 1);
	genotype.setFitness(1.0);
	EXPECT_EQ(genotype.getHistorySize(), 1);
}

TEST(TEST_HistoricalGenotype, HistorySizeIncreasesAfterIteration) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno,1, 1);
	genotype.setFitness(1.0);
	EXPECT_EQ(genotype.getHistorySize(), 1);
	genotype.iterate(inno);
	genotype.setFitness(2.0);
	EXPECT_EQ(genotype.getHistorySize(), 2);
}

TEST(TEST_HistoricalGenotype, HistoricalGenotypeResetsIfNotEnoughImprovementIsMade) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.05);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitness(), 0);
}

TEST(TEST_HistoricalGenotype, HistoricalGenotypeSurvivesIfEnoughImprovementIsMade) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno,1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.1);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitness(), 1.1);
}

TEST(TEST_HistoricalGenotype, highestFitnessAtStartOfCenturyGetsSetCorrectlyAfterEvolutionaryPurge) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.1);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitnessAtStartOfCentury(), 1.1);
}

TEST(TEST_HistoricalGenotype, highestFitnessAtStartOfCenturyGetsSetCorrectlyAfterEvolutionReset) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.05);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitnessAtStartOfCentury(), 0);
}

TEST(TEST_HistoricalGenotype, outputChangesAfterMutation) {
	nev::Innovation inno;
	nev::HistoricalGenotype genotype = nev::HistoricalGenotype(inno, 1, 1);
	genotype.setMutateWeightProbability(1);
	genotype.setNewWeightProbability(1);
	std::vector<double> outputs = genotype.calculateOutputSnapshotFromLastGenotype(std::vector<double> {1.0});
	double previousValue = outputs[0];
	genotype.iterate(inno);
	outputs.clear();
	outputs = genotype.calculateOutputSnapshotFromLastGenotype(std::vector<double> {1.0});

	EXPECT_NE(previousValue, outputs[0]);
}