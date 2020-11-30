#include "HNE/HistoricalGenotype.h"

nev::HistoricalGenotype::HistoricalGenotype(Innovation& inno, int countOfInputs, int countOfOutputs)
{
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;

	genotypeHistory.push_back(Genotype(inno, countOfInputs, countOfOutputs,0));
}

nev::HistoricalGenotype::HistoricalGenotype(Genotype genotype, int countOfInputs, int countOfOutputs)
{
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;

	genotypeHistory.push_back(genotype);

}

std::vector<double> nev::HistoricalGenotype::calculateOutputSnapshotFromLastGenotype(const std::vector<double> &input)
{
//	Genotype geno = ;
	std::vector<double> output = genotypeHistory[genotypeHistory.size() - 1].calculateOutputSnapshot(input);
	return output;
}

std::vector<double> nev::HistoricalGenotype::calculateOutputActiveFromLastGenotype(const std::vector<double>& input)
{
	std::vector<double> output = genotypeHistory[genotypeHistory.size() - 1].calculateOutputActive(input);

	return output;
}

void nev::HistoricalGenotype::setFitness(const double & fitness)
{
	if (genotypeHistory.size() == 0)
		return;

	genotypeHistory[genotypeHistory.size() - 1].setRawFitness(fitness);
}

void nev::HistoricalGenotype::iterate(Innovation& inno)
{
	Genotype currentGeno = genotypeHistory[genotypeHistory.size() - 1];
	currentGeno.setRawFitness(0);
	mutate(inno, currentGeno);
	currentGeno.deletePhenotype();
	genotypeHistory.push_back(currentGeno);
}

void nev::HistoricalGenotype::evolution(Innovation& inno)
{
	if (improvedEnough()) {
		purgeAllExceptHighestPerformingGenotype(inno);
	}
	else {
		reset(inno);
	}
}

void nev::HistoricalGenotype::mutate(Innovation& inno, Genotype &genotype)
{
	genotype.randomlyAddLink(inno, addLinkProbability, recurrentAllowed);
	genotype.randomlyAddNeuron(inno, addNeuronProbability);
	genotype.randomlyMutateAllWeights(mutateWeightProbability, newWeightProbability, maxWeightPertubation);
}

void nev::HistoricalGenotype::deletePhenotype()
{
	for (int i = 0; i < genotypeHistory.size(); i++) {
		genotypeHistory[i].deletePhenotype();
	}
}

void nev::HistoricalGenotype::purgeAllExceptHighestPerformingGenotype(Innovation& inno)
{
	for (int i = 0; i < genotypeHistory.size(); i++)
		genotypeHistory[i].deletePhenotype();

	highestFitnessAtStartOfCentury = getHighestFitness();
	Genotype topPerformer = getHighestPerformingGenotype(inno);
	genotypeHistory.clear();
	genotypeHistory.push_back(topPerformer);
}

bool nev::HistoricalGenotype::improvedEnough()
{
	double fitnessNeeded = highestFitnessAtStartOfCentury + (highestFitnessAtStartOfCentury * improvementNeeded);
	if (fitnessNeeded <= getHighestFitness())
		return true;

	return false;
}

void nev::HistoricalGenotype::reset(Innovation& inno)
{
	genotypeHistory.clear();
	highestFitnessAtStartOfCentury = 0;
	genotypeHistory.push_back(Genotype(inno, countOfInputs, countOfOutputs, 0));
}

int nev::HistoricalGenotype::getHistorySize()
{
	return genotypeHistory.size();
}

double nev::HistoricalGenotype::getHighestFitness()
{
	double highestFitness = 0;
	for (int i = 0; i < genotypeHistory.size(); i++) {
		if (highestFitness < genotypeHistory[i].getRawFitness())
			highestFitness = genotypeHistory[i].getRawFitness();
	}
	return highestFitness;
}

double nev::HistoricalGenotype::getHighestFitnessAtStartOfCentury()
{
	return highestFitnessAtStartOfCentury;
}

void nev::HistoricalGenotype::setImprovementNeeded(double improvementNeeded)
{
	this->improvementNeeded = improvementNeeded;
}

void nev::HistoricalGenotype::setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury)
{
	this->highestFitnessAtStartOfCentury = highestFitnessAtStartOfCentury;
}

nev::Genotype nev::HistoricalGenotype::getHighestPerformingGenotype(Innovation& inno)
{
	std::sort(genotypeHistory.begin(), genotypeHistory.end());

	if (genotypeHistory.size() == 0)
		return Genotype(inno, countOfInputs, countOfOutputs, 0);

	return genotypeHistory[0];
}

void nev::HistoricalGenotype::setAddLinkProbability(double addLinkProbability)
{
	this->addLinkProbability = addLinkProbability;
}

void nev::HistoricalGenotype::setAddNeuronProbability(double addNeuronProbability)
{
	this->addNeuronProbability = addNeuronProbability;
}

void nev::HistoricalGenotype::setMutateWeightProbability(double mutateWeightProbability)
{
	this->mutateWeightProbability = mutateWeightProbability;
}

void nev::HistoricalGenotype::setNewWeightProbability(double newWeightProbability)
{
	this->newWeightProbability = newWeightProbability;
}

void nev::HistoricalGenotype::setMaxWeightPertubation(double maxWeightPertubation)
{
	this->maxWeightPertubation = maxWeightPertubation;
}

void nev::HistoricalGenotype::setRecurrentAllowed(bool recurrentAllowed)
{
	this->recurrentAllowed = recurrentAllowed;
}
