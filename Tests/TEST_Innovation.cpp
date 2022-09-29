#include <gtest/gtest.h>
#include <Innovation.h>
#include <Genotype.h>
#include "Test_config.h"

#if RUN_NORMAL_TESTS

TEST(TEST_Innovation, 0by0GenotypeCreates0Innovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 0, 0, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 0);
}

TEST(TEST_Innovation, 1by1GenotypeCreates2Innovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 2);
}

TEST(TEST_Innovation, 1by1GenotypeCreates2NewLinkInnovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(inno.getCountOfNewLinkInnovations(), 2);
}

TEST(TEST_Innovation, 1by1GenotypeCreates0NewNeuronInnovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 0);
}

TEST(TEST_Innovation, 2_1by1GenotypesCreate2Innovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	auto geno2 = nev::Genotype(&inno, 1, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 2);
}

TEST(TEST_Innovation, 2by1GenotypeCreates3Innovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 3);
}

TEST(TEST_Innovation, 2_2by1GenotypesCreate3Innovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);
	auto geno2 = nev::Genotype(&inno, 2, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 3);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovation)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	geno.randomlyAddNeuron(&inno, 1.f);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovationAndTheSecondNetDoesntAddAInnovation)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	auto geno2 = nev::Genotype(&inno, 1, 1, 2);
	geno.randomlyAddNeuron(&inno, 1.f);
	geno2.randomlyAddNeuron(&inno, 1.f);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovationButOnlyOnce)
{
	auto inno = nev::Innovation();
	for (int i = 0; i < 10; i++)
	{
		auto geno = nev::Genotype(&inno, 1, 1, 2);
		geno.randomlyAddNeuron(&inno, 1.f);
	}

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAdds2MoreNewLinkInnovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousCountLinkInnovations + 2, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addNeuronAdds2MoreNewLinkInnovationsButOnlyOnce)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	for (int i = 0; i < 10; i++)
	{
		geno = nev::Genotype(&inno, 1, 1, 1);
		geno.randomlyAddNeuron(&inno, 1.0);
	}

	EXPECT_EQ(previousCountLinkInnovations + 2, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addNeuronAdds3MoreTotalInnovations)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	int previousTotalInnovations = inno.getTotalInnovationsCount();
	geno = nev::Genotype(&inno, 1, 1, 1);
	geno.randomlyAddNeuron(&inno, 1.0);

	EXPECT_EQ(previousTotalInnovations + 3, inno.getTotalInnovationsCount());
}

TEST(TEST_Innovation, addNeuronAdds3MoreTotalInnovationsOnlyOnce)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 1, 1, 1);
	int previousTotalInnovations = inno.getTotalInnovationsCount();
	for (int i = 0; i < 10; i++)
	{
		geno = nev::Genotype(&inno, 1, 1, 1);
		geno.randomlyAddNeuron(&inno, 1.0);
	}

	EXPECT_EQ(previousTotalInnovations + 3, inno.getTotalInnovationsCount());
}

TEST(TEST_Innovation, addLinkRecurrentAllowedAddsNewLinkInnovation)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);
	geno.randomlyAddNeuron(&inno, 1.0);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	geno.randomlyAddLink(&inno, 1.0, true);

	EXPECT_EQ(previousCountLinkInnovations + 1, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addLinkAddsNewLinkInnovationsOnlyOneTime)
{
	auto inno = nev::Innovation();
	auto geno = nev::Genotype(&inno, 2, 1, 1);
	geno.randomlyAddNeuron(&inno, 1.0);
	auto geno2 = geno;
	for (int i = 0; i < 100; i++)
		geno.randomlyAddLink(&inno, 1.0, false);

	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	for (int i = 0; i < 100; i++)
		geno.randomlyAddLink(&inno, 1.0, false);

	EXPECT_EQ(previousCountLinkInnovations, inno.getCountOfNewLinkInnovations());
}

#endif