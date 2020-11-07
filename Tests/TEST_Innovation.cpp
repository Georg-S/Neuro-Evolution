#include <gtest/gtest.h>
#include <Innovation.h>
#include <Genotype.h>


TEST(TEST_Innovation, 0by0GenotypeCreates0Innovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 0, 0, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 0);
}

TEST(TEST_Innovation, 1by1GenotypeCreates2Innovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 2);
}

TEST(TEST_Innovation, 1by1GenotypeCreates2NewLinkInnovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(inno.getCountOfNewLinkInnovations(), 2);
}

TEST(TEST_Innovation, 1by1GenotypeCreates0NewNeuronInnovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 0);
}

TEST(TEST_Innovation, 2_1by1GenotypesCreate2Innovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = Genotype(inno, 1, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 2);
}

TEST(TEST_Innovation, 2by1GenotypeCreates3Innovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 3);
}

TEST(TEST_Innovation, 2_2by1GenotypesCreate3Innovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);
	Genotype geno2 = Genotype(inno, 2, 1, 1);

	EXPECT_EQ(inno.getTotalInnovationsCount(), 3);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovation) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	geno.randomlyAddNeuron(inno, 1.f);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovationAndTheSecondNetDoesntAddAInnovation) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	Genotype geno2 = Genotype(inno, 1, 1, 2);
	geno.randomlyAddNeuron(inno, 1.f);
	geno2.randomlyAddNeuron(inno, 1.f);

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAddsNewNeuronInnovationAndButOnlyOnce) {
	Innovation inno = Innovation();
	for (int i = 0; i < 10; i++) {
		Genotype geno = Genotype(inno, 1, 1, 2);
		geno.randomlyAddNeuron(inno, 1.f);
	}

	EXPECT_EQ(inno.getCountOfNewNeuronInnovations(), 1);
}

TEST(TEST_Innovation, addNeuronAdds2MoreNewLinkInnovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousCountLinkInnovations + 2, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addNeuronAdds2MoreNewLinkInnovationsButOnlyOnce) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	for (int i = 0; i < 10; i++) {
		geno = Genotype(inno, 1, 1, 1);
		geno.randomlyAddNeuron(inno, 1.0);
	}
	EXPECT_EQ(previousCountLinkInnovations + 2, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addNeuronAdds3MoreTotalInnovations) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	int previousTotalInnovations = inno.getTotalInnovationsCount();
	geno = Genotype(inno, 1, 1, 1);
	geno.randomlyAddNeuron(inno, 1.0);

	EXPECT_EQ(previousTotalInnovations + 3, inno.getTotalInnovationsCount());
}

TEST(TEST_Innovation, addNeuronAdds3MoreTotalInnovationsOnlyOnce) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 1, 1, 1);
	int previousTotalInnovations = inno.getTotalInnovationsCount();
	for (int i = 0; i < 10; i++) {
		geno = Genotype(inno, 1, 1, 1);
		geno.randomlyAddNeuron(inno, 1.0);
	}
	EXPECT_EQ(previousTotalInnovations + 3, inno.getTotalInnovationsCount());
}

TEST(TEST_Innovation, addLinkAddsNewLinkInnovation) {
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);
	geno.randomlyAddNeuron(inno, 1.0);
	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousCountLinkInnovations + 1, inno.getCountOfNewLinkInnovations());
}

TEST(TEST_Innovation, addLinkAddsNewLinkInnovationsOnlyOneTime) {
	std::srand(time(NULL));
	Innovation inno = Innovation();
	Genotype geno = Genotype(inno, 2, 1, 1);
	geno.randomlyAddNeuron(inno, 1.0);
	Genotype geno2 = geno;
	for(int i = 0; i < 100; i++) 
		geno.randomlyAddLink(inno, 1.0, false);

	int previousCountLinkInnovations = inno.getCountOfNewLinkInnovations();
	for (int i = 0; i < 100; i++)
		geno.randomlyAddLink(inno, 1.0, false);

	EXPECT_EQ(previousCountLinkInnovations, inno.getCountOfNewLinkInnovations());
}
