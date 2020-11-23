#include <gtest/gtest.h>
#include <Genotype.h>
#include <Innovation.h>
#include <Species.h>
#include <vector>

TEST(TEST_Species, calculateCompatibilityScoreCalculates0ForTheSameGenotype) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);
	nev::Genotype geno2 = geno;
	nev::Species specie = nev::Species(geno, 1);

	EXPECT_EQ(specie.calculateCompatibilityScore(geno2, 1, 1, 0.4), 0);
}

TEST(TEST_Species, calculateCompatibilityScoreCalculates2For2InnovationsMore) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype geno = nev::Genotype(inno, 1, 1, 1);
	nev::Genotype geno2 = geno;
	geno.randomlyAddNeuron(inno, 1.0);
	nev::Species specie = nev::Species(geno, 1);

	EXPECT_EQ(specie.calculateCompatibilityScore(geno2, 1, 1, 0.4), 2);
}

TEST(TEST_Species, calculateAdjustedFitnessCalculatesRightTotalFitness) {
	nev::Innovation inno = nev::Innovation();
	nev::Genotype genotype = nev::Genotype(inno,1,1,1);
	genotype.setRawFitness(50);
	nev::Species specie = nev::Species(genotype,1);

	for (int i = 0; i < 49;i++) {
		specie.addMemberToSpecies(&genotype);
	}
	specie.updateFitnessValues();

	EXPECT_EQ(specie.getTotalCurrentAdjustedFitness(), 50);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::Genotype> population;
	int fitness = 100;
	int id = 0;
	for (int i = 0; i < 50; i++) {
		fitness--;
		nev::Genotype genotype = nev::Genotype(inno, 1, 1, id++);
		genotype.setRawFitness(fitness);
		population.push_back(genotype);
	}
	nev::Species specie = nev::Species(population[0], 1);
	for (int i = 1; i < population.size(); i++) {
		specie.addMemberToSpecies(&population[i]);
	}
	

	EXPECT_EQ(specie.getLeader().getGenotypeId(),0);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype_2) {
	nev::Innovation inno = nev::Innovation();
	std::vector<nev::Genotype> population;
	int fitness = 100;
	int id = 0;
	for (int i = 0; i < 50; i++) {
		fitness++;
		nev::Genotype genotype = nev::Genotype(inno, 1, 1, id++);
		genotype.setRawFitness(fitness);
		population.push_back(genotype);
	}
	nev::Species specie = nev::Species(population[0], 1);
	for (int i = 1; i < population.size(); i++) {
		specie.addMemberToSpecies(&population[i]);
	}


	EXPECT_EQ(specie.getLeader().getGenotypeId(), 49);
}