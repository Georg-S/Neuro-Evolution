#include <gtest/gtest.h>
#include <memory>
#include <Genotype.h>
#include <Innovation.h>
#include <Species.h>
#include <vector>

TEST(TEST_Species, calculateCompatibilityScoreCalculates0ForTheSameGenotype) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	nev::Species specie = nev::Species(geno, 1);

	EXPECT_EQ(specie.calculateCompatibilityScore(geno, 1, 1, 0.4), 0);
}

TEST(TEST_Species, calculateCompatibilityScoreCalculates2For2InnovationsMore) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> geno = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	auto geno2 = std::make_shared<nev::Genotype>(*geno.get());
	geno->randomlyAddNeuron(inno, 1.0);
	nev::Species specie = nev::Species(geno, 1);

	EXPECT_EQ(specie.calculateCompatibilityScore(geno2, 1, 1, 0.4), 2);
}

TEST(TEST_Species, calculateAdjustedFitnessCalculatesRightTotalFitness) {
	nev::Innovation inno = nev::Innovation();
	std::shared_ptr<nev::Genotype> genotype = std::make_shared<nev::Genotype>(inno, 1, 1, 1);
	genotype->setRawFitness(50);
	nev::Species species = nev::Species(genotype, 1);

	for (int i = 0; i < 49; i++) {
		species.addMemberToSpecies(genotype.get());
	}
	species.updateFitnessValues();

	EXPECT_EQ(species.getTotalCurrentAdjustedFitness(), 50);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype) {
	nev::Innovation inno = nev::Innovation();
	std::vector<std::shared_ptr<nev::Genotype>> population;
	int fitness = 100;
	int id = 0;
	for (int i = 0; i < 50; i++) {
		fitness--;
		std::shared_ptr<nev::Genotype> genotype = std::make_shared<nev::Genotype>(inno, 1, 1, id++);
		genotype->setRawFitness(fitness);
		population.push_back(genotype);
	}
	nev::Species specie = nev::Species(population[0], 1);
	for (int i = 1; i < population.size(); i++) {
		specie.addMemberToSpecies(population[i].get());
	}


	EXPECT_EQ(specie.getDeepCopyOfSpeciesLeader()->getGenotypeId(), 0);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype_2) {
	nev::Innovation inno = nev::Innovation();
	std::vector<std::shared_ptr<nev::Genotype>> population;
	int fitness = 100;
	int id = 0;
	for (int i = 0; i < 50; i++) {
		fitness++;
		std::shared_ptr<nev::Genotype> genotype = std::make_shared<nev::Genotype>(inno, 1, 1, id++);
		genotype->setRawFitness(fitness);
		population.push_back(genotype);
	}
	nev::Species specie = nev::Species(population[0], 1);
	for (int i = 1; i < population.size(); i++) {
		specie.addMemberToSpecies(population[i].get());
	}


	EXPECT_EQ(specie.getDeepCopyOfSpeciesLeader()->getGenotypeId(), 49);
}