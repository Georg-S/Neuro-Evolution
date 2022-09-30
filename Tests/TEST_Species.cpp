#include <gtest/gtest.h>
#include <memory>
#include <Genotype.h>
#include <Innovation.h>
#include <Species.h>
#include <vector>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_Species, calculateCompatibilityScoreCalculates0ForTheSameGenotype)
{
	auto inno = nev::Innovation();
	auto genotype = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto specie = nev::Species(genotype.get(), 1);

	EXPECT_EQ(specie.calculateCompatibilityScore(genotype.get(), 1, 1, 0.4), 0);
}

TEST(TEST_Species, calculateCompatibilityScoreCalculates2For2InnovationsMore)
{
	auto inno = nev::Innovation();
	auto genotype = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	auto genotype2 = std::make_shared<nev::Genotype>(*genotype);
	genotype->randomlyAddNeuron(&inno, 1.0);
	auto species = nev::Species(genotype.get(), 1);

	EXPECT_EQ(species.calculateCompatibilityScore(genotype2.get(), 1, 1, 0.4), 2);
}

TEST(TEST_Species, calculateAdjustedFitnessCalculatesRightTotalFitness)
{
	auto inno = nev::Innovation();
	auto genotype = std::make_unique<nev::Genotype>(&inno, 1, 1, 1);
	genotype->setRawFitness(50);
	auto species = nev::Species(genotype.get(), 1);

	for (int i = 0; i < 49; i++)
		species.addMemberToSpecies(genotype.get());

	species.updateFitnessValues();

	EXPECT_EQ(species.getTotalCurrentAdjustedFitness(), 50);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype)
{
	auto inno = nev::Innovation();
	std::vector<std::shared_ptr<nev::Genotype>> population;
	int fitness = 100;
	int id = 0;

	for (int i = 0; i < 50; i++)
	{
		fitness--;
		auto genotype = std::make_shared<nev::Genotype>(&inno, 1, 1, id++);
		genotype->setRawFitness(fitness);
		population.emplace_back(genotype);
	}

	auto specie = nev::Species(population[0].get(), 1);
	for (int i = 1; i < population.size(); i++)
		specie.addMemberToSpecies(population[i].get());

	EXPECT_EQ(specie.getDeepCopyOfSpeciesLeader()->getGenotypeId(), 0);
}

TEST(TEST_Species, getLeaderReturnTheRightGenotype_2)
{
	auto inno = nev::Innovation();
	std::vector<std::unique_ptr<nev::Genotype>> population;
	int fitness = 100;
	int id = 0;
	for (int i = 0; i < 50; i++)
	{
		fitness++;
		auto genotype = std::make_unique<nev::Genotype>(&inno, 1, 1, id++);
		genotype->setRawFitness(fitness);
		population.emplace_back(std::move(genotype));
	}
	auto specie = nev::Species(population.front().get(), 1);
	for (int i = 1; i < population.size(); i++)
		specie.addMemberToSpecies(population[i].get());

	EXPECT_EQ(specie.getDeepCopyOfSpeciesLeader()->getGenotypeId(), 49);
}

#endif