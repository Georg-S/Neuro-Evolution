#include "Genotype.h"


Genotype::Genotype()
{
}

Genotype::Genotype(Innovation & innovation, int countOfInputs, int countOfOutputs, int id)
{
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	this->id = id;
	createFullyConnectedNetwork(innovation);
}

Genotype::Genotype(std::vector<NeuronGene> neurons, std::vector<LinkGene> links, int id)
{
	this->neurons = neurons;
	this->links = links;
	this->id = id;
	int inputs = 0;
	int outputs = 0;
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == input)
			inputs++;

		if (neurons[i].neuronType == output)
			outputs++;
	}
	this->countOfInputs = inputs;
	this->countOfOutputs = outputs;
	calculateDepthOfEveryNeuron();
}

Genotype::Genotype(Innovation &innovation, std::vector<NeuronGene> neurons, std::vector<LinkGene> links, int id)
{
	int inputs = 0;
	int outputs = 0;
	int highestNeuronId = 0;
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == input)
			inputs++;

		if (neurons[i].neuronType == output)
			outputs++;

		if (neurons[i].id > highestNeuronId)
			highestNeuronId = neurons[i].id;
	}
	this->countOfInputs = inputs;
	this->countOfOutputs = outputs;
	this->neurons = neurons;
	this->links = links;
	this->id = id;
	innovation.setCurrentNeuronId(highestNeuronId);
	calculateDepthOfEveryNeuron();
}


Genotype::~Genotype()
{
}

void Genotype::randomlyAddNeuron(Innovation &innovation, float addNeuronProbability)
{
	if (addNeuronProbability < RNG::getRandomFloatBetween0and1())
		return;

	int trys = numTrysToAddNeuron;
	int linkIndex = -1;
	while (trys--) {
		int randomLinkIndex = RNG::getRandomIntBetween(0, links.size() - 1);
		if (isValidLinkIndexForAddNeuron(randomLinkIndex)) {
			linkIndex = randomLinkIndex;
			trys = 0;
		}
	}

	if (linkIndex == -1)
		return;

	int fromId = links[linkIndex].fromNeuronID;
	int toId = links[linkIndex].toNeuronID;

	int newNeuronId = innovation.getNeuronId(links[linkIndex].fromNeuronID, links[linkIndex].toNeuronID);
	if (newNeuronId == -1)
		newNeuronId = innovation.createNewNeuronInnovation(fromId, toId);
	neurons.push_back(NeuronGene(hidden, newNeuronId));

	createLink(innovation, fromId, newNeuronId,false,1.0);
	createLink(innovation, newNeuronId, toId,false,links[linkIndex].weight);
	links[linkIndex].enabled = false;
	calculateDepthOfEveryNeuron();
}

void Genotype::randomlyMutateAllWeights(float mutationProbability, float newWeightProbability, float weightPertubation)
{
	for (int i = 0; i < links.size(); i++) {
		if (mutationProbability < RNG::getRandomFloatBetween0and1())
			continue;

		mutateSingleWeight(newWeightProbability, i, weightPertubation);
	}
}

void Genotype::randomlyAddLink(Innovation & innovation, float mutationProbability, bool recurrentAllowed)
{
	if (mutationProbability < RNG::getRandomFloatBetween0and1())
		return;

	int trysToFindValidIndizes = numTrysToAddLink;
	int fromIndex = -1;
	int toIndex = -1;

	while (trysToFindValidIndizes--) {
		int randomFromIndex = RNG::getRandomIntBetween(0, neurons.size() - 1);
		int randomToIndex = RNG::getRandomIntBetween(0, neurons.size() - 1);

		if (areValidNeuronIndizesForAddNeuron(randomFromIndex, randomToIndex, recurrentAllowed)) {
			fromIndex = randomFromIndex;
			toIndex = randomToIndex;
			trysToFindValidIndizes = 0;
		}
	}

	if (fromIndex == -1 || toIndex == -1)
		return;

	createLinkWithRandomWeight(innovation, neurons[fromIndex].id, neurons[toIndex].id,isRecurrentBetweenNodes(fromIndex,toIndex));
}

double Genotype::calculateCompatibilityScore(Genotype & partner, const float &exzessFactor,
											const float &disjointFactor, const float &weightFactor)
{
	sort(partner.links.begin(), partner.links.end());
	sort(links.begin(), links.end());
	int countOfExzess = 0;
	int countOfCommon = 0;
	int countOfDisjoint = 0;
	double totalWeightDifference = 0;

	int genotypeIndex = 0;
	int partnerIndex = 0;

	while (genotypeIndex < links.size() || partnerIndex < partner.links.size()) {
		if (genotypeIndex >= links.size()) {
			partnerIndex++;
			countOfExzess++;
		}
		else if (partnerIndex >= partner.links.size()) {
			genotypeIndex++;
			countOfExzess++;
		}
		else if (links[genotypeIndex].innovationID == partner.links[partnerIndex].innovationID) {
			countOfCommon++;
			totalWeightDifference += abs(links[genotypeIndex].weight - partner.links[partnerIndex].weight);
			genotypeIndex++;
			partnerIndex++;
		}
		else if (links[genotypeIndex].innovationID > partner.links[partnerIndex].innovationID) {
			countOfDisjoint++;
			partnerIndex++;
		}
		else if (links[genotypeIndex].innovationID < partner.links[partnerIndex].innovationID) {
			countOfDisjoint++;
			genotypeIndex++;
		}
	}
	int maxCountOfGenes = links.size();
	if (partner.links.size() > maxCountOfGenes)
		maxCountOfGenes = partner.links.size();
	if (maxCountOfGenes < 20)
		maxCountOfGenes = 1;


	double compatibilityScore = ((countOfExzess * exzessFactor) / maxCountOfGenes)
		+ ((countOfDisjoint * disjointFactor) / maxCountOfGenes)
		+ ((totalWeightDifference / countOfCommon) * weightFactor);


	return compatibilityScore;
}

Genotype Genotype::crossOver(Genotype & mother, int babyId)
{
	std::vector<LinkGene> fatherLinks = this->links;
	std::vector<LinkGene> motherLinks = mother.links;
	int fatherLinkIndex = 0;
	int motherLinkIndex = 0;
	sort(fatherLinks.begin(), fatherLinks.end());
	sort(motherLinks.begin(), motherLinks.end());
	LinkGene currentGene;
	NeuronGene fromNeuron;
	NeuronGene toNeuron;
	std::vector<LinkGene> babyLinks;
	std::vector<NeuronGene> babyNeurons;

	ParentType highestFitness = Father;
	if (mother.rawFitness > rawFitness)
		highestFitness = Mother;
	if (mother.rawFitness == rawFitness) {
		if (RNG::getRandomIntBetween(0, 1) == 0)
			highestFitness = Mother;
	}

	while ((motherLinkIndex < motherLinks.size()) || (fatherLinkIndex < fatherLinks.size())) {
		if (fatherLinkIndex >= fatherLinks.size()) {
			if (highestFitness == Mother) {
				currentGene = motherLinks[motherLinkIndex];
				fromNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].fromNeuronID);
				toNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].toNeuronID);
			}
			motherLinkIndex++;
		}
		else if (motherLinkIndex >= motherLinks.size()) {
			if (highestFitness == Father) {
				currentGene = fatherLinks[fatherLinkIndex];
				fromNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].fromNeuronID);
				toNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].toNeuronID);
			}
			fatherLinkIndex++;
		}
		else if (motherLinks[motherLinkIndex].innovationID > fatherLinks[fatherLinkIndex].innovationID) {
			if (highestFitness == Father) {
				currentGene = fatherLinks[fatherLinkIndex];
				fromNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].fromNeuronID);
				toNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].toNeuronID);
			}
			fatherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID > motherLinks[motherLinkIndex].innovationID) {
			if (highestFitness == Mother) {
				currentGene = motherLinks[motherLinkIndex];
				fromNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].fromNeuronID);
				toNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].toNeuronID);
			}
			motherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID == motherLinks[motherLinkIndex].innovationID) {
			double weight = 0;
			if (highestFitness == Mother) {
				currentGene = motherLinks[motherLinkIndex];
				fromNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].fromNeuronID);
				toNeuron = mother.getNeuronGeneFromId(motherLinks[motherLinkIndex].toNeuronID);
			}
			if (highestFitness == Father) {
				currentGene = fatherLinks[fatherLinkIndex];
				fromNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].fromNeuronID);
				toNeuron = getNeuronGeneFromId(fatherLinks[fatherLinkIndex].toNeuronID);
			}

			if (RNG::getRandomIntBetween(0, 1) == 0)
				weight = motherLinks[motherLinkIndex].weight;
			else
				weight = fatherLinks[fatherLinkIndex].weight;

			currentGene.weight = weight;
			motherLinkIndex++;
			fatherLinkIndex++;
		}
		addLinkToVectorIfNotAlreadyInside(currentGene, babyLinks);
		addNeuronToVectorIfNotAlreadyInside(fromNeuron, babyNeurons);
		addNeuronToVectorIfNotAlreadyInside(toNeuron, babyNeurons);
	}
	Genotype baby = Genotype(babyNeurons, babyLinks, babyId);
	return baby;
}

std::vector<double> Genotype::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputSnapshot(inputs);
}

std::vector<double> Genotype::calculateOutputActive(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputActive(inputs);
}

void Genotype::createPhenotype()
{
	deletePhenotype();

	std::vector<PhenotypeNeuron*> phenoNeurons;
	for (int i = 0; i < neurons.size(); i++) {
		PhenotypeNeuron* phenoNeuron = new PhenotypeNeuron(neurons[i].neuronType, neurons[i].id);
		phenoNeurons.push_back(phenoNeuron);
	}

	for (int i = 0; i < links.size(); i++) {
		if (!links[i].enabled)
			continue;

		int fromIndex = getNeuronIndexFromId(links[i].fromNeuronID);
		int toIndex = getNeuronIndexFromId(links[i].toNeuronID);
		PhenotypeNeuron* fromNeuron = phenoNeurons[fromIndex];
		PhenotypeNeuron* toNeuron = phenoNeurons[toIndex];
		toNeuron->linksIn.push_back(PhenotypeLink(fromNeuron, toNeuron, links[i].weight));
	}

	phenotype = new Phenotype(phenoNeurons, maxDepth);
}

void Genotype::deletePhenotype()
{
	if (phenotype != nullptr)
		delete phenotype;

	phenotype = nullptr;
}

void Genotype::setAdjustedFitness(double fitness)
{
	this->adjustedFitness = fitness;
}

void Genotype::setRawFitness(double fitness)
{
	this->rawFitness = fitness;
}

double Genotype::getAdjustedFitness()
{
	return adjustedFitness;
}

double Genotype::getRawFitness()
{
	return rawFitness;
}

int Genotype::getCountOfNeurons()
{
	return neurons.size();
}

int Genotype::getCountOfLinks()
{
	return links.size();
}

int Genotype::getMaxDepth()
{
	return maxDepth;
}

double Genotype::getLinkWeightAverage()
{
	double total = 0;
	if (links.size() == 0)
		return 0;

	for (int i = 0; i < links.size(); i++) {
		total += links[i].weight;
	}
	return total/links.size();
}

int Genotype::getGenotypeId()
{
	return id;
}

int Genotype::getCountOfInputs()
{
	return countOfInputs;
}

int Genotype::getCountOfOutputs()
{
	return countOfOutputs;
}

std::vector<NeuronGene> Genotype::getNeurons()
{
	return neurons;
}

std::vector<LinkGene> Genotype::getLinks()
{
	return links;
}

void Genotype::mutateSingleWeight(float newWeightProbability, int linkIndex, float weightPertubation)
{
	if (newWeightProbability > RNG::getRandomFloatBetween0and1()) {
		links[linkIndex].weight = getRandomLinkWeight();
	}
	else {
		float pertubationAmount = weightPertubation;
		if (RNG::getRandomIntBetween(0, 1) == 0)
			pertubationAmount *= -1;

		links[linkIndex].weight += pertubationAmount;
	}
	if (links[linkIndex].weight < minimumLinkWeight)
		links[linkIndex].weight = minimumLinkWeight;

	if (links[linkIndex].weight > maximumLinkWeight)
		links[linkIndex].weight = maximumLinkWeight;
}

NeuronGene Genotype::getNeuronGeneFromId(int id)
{
	int neuronIndex = getNeuronIndexFromId(id);
	if (neuronIndex != -1)
		return neurons[neuronIndex];

	return NeuronGene();
}

void Genotype::addLinkToVectorIfNotAlreadyInside(const LinkGene & link, std::vector<LinkGene>& linkVec)
{
	for (int i = 0; i < linkVec.size(); i++) {
		if ((linkVec[i].fromNeuronID == link.fromNeuronID) && (linkVec[i].toNeuronID == link.toNeuronID))
			return;
	}
	linkVec.push_back(link);
}

void Genotype::addNeuronToVectorIfNotAlreadyInside(const NeuronGene & neuron, std::vector<NeuronGene>& neuronVec)
{
	for (int i = 0; i < neuronVec.size(); i++) {
		if (neuron.id == neuronVec[i].id)
			return;
	}
	neuronVec.push_back(neuron);
}

void Genotype::createFullyConnectedNetwork(Innovation &innovation)
{
	if (countOfInputs <= 0 || countOfOutputs <= 0)
		return;

	createNeurons();
	createLinks(innovation);
	calculateDepthOfEveryNeuron();
}

void Genotype::createNeurons()
{
	neurons.push_back(NeuronGene(bias, 0));

	int neuronId = 1;
	for (int i = 0; i < countOfInputs; i++) {
		neurons.push_back(NeuronGene(input, neuronId));
		neuronId++;
	}

	for (int i = 0; i < countOfOutputs; i++) {
		neurons.push_back(NeuronGene(output, neuronId));
		neuronId++;
	}
}

void Genotype::createLinks(Innovation &innovation)
{
	for (int i = 0; i <= countOfInputs; i++) {
		for (int x = 0; x < countOfOutputs; x++) {
			int fromId = i;
			int toId = neurons.size() - 1 - x;

			createLinkWithRandomWeight(innovation, fromId, toId,false);
		}
	}
}

void Genotype::calculateDepthOfEveryNeuron()
{
	if (neurons.size() == 0 || countOfInputs == 0 || countOfOutputs == 0)
		return;

	maxDepth = -1;
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == bias || neurons[i].neuronType == input)
			neurons[i].depth = 0;
		else
			neurons[i].depth = -1;
	}

	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == bias || neurons[i].neuronType == input)
			updateDepthOfNeuronsConnectedToThis(i);
	}
}

void Genotype::updateDepthOfNeuronsConnectedToThis(int neuronIndex)
{
	for (int i = 0; i < links.size(); i++) {
		if (!links[i].enabled || links[i].recurrent)
			continue;

		int fromIndex = neuronIndex;
		if (links[i].fromNeuronID == neurons[fromIndex].id) {
			int toIndex = getNeuronIndexFromId(links[i].toNeuronID);
			if (neurons[toIndex].depth <= neurons[fromIndex].depth) {
				int newDepth = neurons[fromIndex].depth + 1;
				neurons[toIndex].depth = newDepth;

				if (newDepth > maxDepth)
					maxDepth = newDepth;

				updateDepthOfNeuronsConnectedToThis(toIndex);
			}
		}
	}
}

bool Genotype::isValidLinkIndexForAddNeuron(int index)
{
	if (links.size() == 0)
		return false;

	int fromIndex = getNeuronIndexFromId(links[index].fromNeuronID);
	if (fromIndex == -1 || !links[index].enabled || links[index].recurrent || neurons[fromIndex].neuronType == bias)
		return false;

	return true;
}

bool Genotype::areValidNeuronIndizesForAddNeuron(int fromIndex, int toIndex, bool recurrentAllowed)
{
	if (neurons.size() == 0)
		return false;

	if (neurons[fromIndex].neuronType == bias && neurons[toIndex].neuronType == input ||
		neurons[fromIndex].neuronType == input && neurons[toIndex].neuronType == bias ||
		neurons[fromIndex].neuronType == bias && neurons[toIndex].neuronType == bias)
		return false;

	if (doesLinkAlreadyExist(fromIndex, toIndex))
		return false;

	if (recurrentAllowed)
		return true;
	else if (isRecurrentBetweenNodes(fromIndex, toIndex))
		return false;

	return true;
}

bool Genotype::doesLinkAlreadyExist(int fromIndex, int toIndex)
{
	int fromId = neurons[fromIndex].id;
	int toId = neurons[toIndex].id;

	for (int i = 0; i < links.size(); i++) {
		if (links[i].fromNeuronID == fromId && links[i].toNeuronID == toId)
			return true;
	}
	return false;
}

bool Genotype::isRecurrentBetweenNodes(int fromIndex, int toIndex)
{
	if (neurons[fromIndex].depth >= neurons[toIndex].depth)
		return true;

	return false;
}

int Genotype::getNeuronIndexFromId(int id)
{
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].id == id)
			return i;
	}
	return -1;
}

double Genotype::getRandomLinkWeight()
{
	return RNG::getRandomFloatBetween(minimumLinkStartValue,maximumLinkStartValue);
}

void Genotype::createLinkWithRandomWeight(Innovation & innovation, int fromId, int toId, bool recurrent)
{
	createLink(innovation, fromId, toId, recurrent, RNG::getRandomFloatBetween(minimumLinkStartValue, maximumLinkStartValue));
}

void Genotype::createLink(Innovation & innovation, int fromId, int toId, bool recurrent, double weightOfLink)
{
	int innovationId = innovation.getInnovationID(fromId, toId, newLink);

	if (innovationId == -1)
		innovationId = innovation.createNewLinkInnovation(fromId, toId);

	links.push_back(LinkGene(fromId, toId, weightOfLink, true, innovationId, recurrent));
}

bool operator<(const Genotype& lhs, const Genotype& rhs)
{
	return lhs.rawFitness > rhs.rawFitness;
}
