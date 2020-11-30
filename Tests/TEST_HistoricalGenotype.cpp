#include <gtest/gtest.h>
#include <HNE/HNE.h>

using namespace std;
using namespace nev;

TEST(HistoricalGenotype, HistorySizeIs1AfterCreation) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno, 1, 1);
	genotype.setFitness(1.0);
	EXPECT_EQ(genotype.getHistorySize(), 1);
}

TEST(HistoricalGenotype, HistorySizeIncreasesAfterIteration) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno,1, 1);
	genotype.setFitness(1.0);
	EXPECT_EQ(genotype.getHistorySize(), 1);
	genotype.iterate(inno);
	genotype.setFitness(2.0);
	EXPECT_EQ(genotype.getHistorySize(), 2);
}

TEST(HistoricalGenotype, HistoricalGenotypeResetsIfNotEnoughImprovementIsMade) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.05);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitness(), 0);
}

TEST(HistoricalGenotype, HistoricalGenotypeSurvivesIfEnoughImprovementIsMade) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno,1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.1);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitness(), 1.1);
}

TEST(HistoricalGenotype, highestFitnessAtStartOfCenturyGetsSetCorrectlyAfterEvolutionaryPurge) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.1);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitnessAtStartOfCentury(), 1.1);
}

TEST(HistoricalGenotype, highestFitnessAtStartOfCenturyGetsSetCorrectlyAfterEvolutionReset) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno, 1, 1);
	genotype.setHighestFitnessAtStartOfCentury(1.0);
	genotype.iterate(inno);
	genotype.setFitness(1.05);
	genotype.evolution(inno);

	EXPECT_EQ(genotype.getHighestFitnessAtStartOfCentury(), 0);
}

TEST(HistoricalGenotype, outputChangesAfterMutation) {
	Innovation inno;
	HistoricalGenotype genotype = HistoricalGenotype(inno, 1, 1);
	genotype.setMutateWeightProbability(1);
	vector<double> outputs = genotype.calculateOutputSnapshotFromLastGenotype(vector<double> {1.0});
	double previousValue = outputs[0];
	genotype.iterate(inno);
	outputs.clear();
	outputs = genotype.calculateOutputSnapshotFromLastGenotype(vector<double> {1.0});

	EXPECT_NE(previousValue, outputs[0]);
}