#include "Genotype.h"


Genotype::Genotype()
{
}

Genotype::Genotype(Innovation& innovation, const int& countOfInputs, const int& countOfOutputs, 
	const int& id, std::function<double(const double& input)> activationFunction)
{
	this->activationFunction = activationFunction;
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	this->id = id;
	createFullyConnectedNetwork(innovation);
}

Genotype::Genotype(const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links, 
	const int& id, std::function<double(const double& input)> activationFunction)
{
	this->activationFunction = activationFunction;
	this->neurons = neurons;
	this->links = links;
	this->id = id;
	this->countOfInputs = 0;
	this->countOfOutputs = 0;

	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == NeuronType::input)
			this->countOfInputs++;

		if (neurons[i].neuronType == NeuronType::output)
			this->countOfOutputs++;
	}

	calculateDepthOfEveryNeuron();
}

Genotype::Genotype(Innovation& innovation, const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links, 
	const int& id, std::function<double(const double& input)> activationFunction)
{
	this->activationFunction = activationFunction;
	this->countOfInputs = 0;
	this->countOfOutputs = 0;
	int highestNeuronId = 0;
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].neuronType == NeuronType::input)
			this->countOfInputs++;

		if (neurons[i].neuronType == NeuronType::output)
			this->countOfOutputs++;

		if (neurons[i].id > highestNeuronId)
			highestNeuronId = neurons[i].id;
	}

	this->neurons = neurons;
	this->links = links;
	this->id = id;
	innovation.setCurrentNeuronId(highestNeuronId);
	calculateDepthOfEveryNeuron();
}

void Genotype::randomlyAddNeuron(Innovation& innovation, const float& addNeuronProbability)
{
	if (links.size() == 0)
		return;

	if (RNG::getRandomFloatBetween0and1() > addNeuronProbability)
		return;

	int linkIndex = -1;
	for (int i = 0; i < numTriesToAddNeuron; i++) {
		int randomLinkIndex = RNG::getRandomIntBetween(0, links.size() - 1);
		if (isValidLinkForAddNeuron(links[randomLinkIndex])) {
			linkIndex = randomLinkIndex;
			break;
		}
	}

	if (linkIndex == -1)
		return;

	int fromId = links[linkIndex].fromNeuronID;
	int toId = links[linkIndex].toNeuronID;

	int newNeuronId = innovation.getNeuronId(links[linkIndex].fromNeuronID, links[linkIndex].toNeuronID);
	if (newNeuronId == -1)
		newNeuronId = innovation.createNewNeuronInnovation(fromId, toId);
	neurons.push_back(NeuronGene(NeuronType::hidden, newNeuronId));

	createLink(innovation, fromId, newNeuronId, false, 1.0);
	createLink(innovation, newNeuronId, toId, false, links[linkIndex].weight);
	links[linkIndex].enabled = false;
	calculateDepthOfEveryNeuron();
}

void Genotype::randomlyMutateAllWeights(const float& mutationProbability, const float& newWeightProbability, const double& weightPertubation)
{
	for (int i = 0; i < links.size(); i++) {
		if (RNG::getRandomFloatBetween0and1() < mutationProbability)
			mutateSingleWeight(newWeightProbability, links[i], weightPertubation);
	}
}

void Genotype::mutateSingleWeight(const float& newWeightProbability, LinkGene& link, const float& weightPertubation)
{
	if (RNG::getRandomFloatBetween0and1() <= newWeightProbability)
		link.weight = getRandomLinkWeight();
	else {
		double pertubationAmount = weightPertubation;
		if (RNG::getRandomIntBetween(0, 1) == 0)
			pertubationAmount *= -1;

		link.weight += pertubationAmount;
	}
	link.weight = std::min(link.weight, maximumLinkWeight);
	link.weight = std::max(link.weight, minimumLinkWeight);
}

void Genotype::randomlyAddLink(Innovation& innovation, const float& mutationProbability, const bool& recurrentAllowed)
{
	if (neurons.size() == 0)
		return;
	if (RNG::getRandomFloatBetween0and1() > mutationProbability)
		return;

	int fromIndex = -1;
	int toIndex = -1;

	for (int triesToFindValidIndizes = 0; triesToFindValidIndizes < numTriesToAddLink; triesToFindValidIndizes++) {
		int randomFromIndex = RNG::getRandomIntBetween(0, neurons.size() - 1);
		int randomToIndex = RNG::getRandomIntBetween(0, neurons.size() - 1);

		if (areValidNeuronsForAddLink(neurons[randomFromIndex], neurons[randomToIndex], recurrentAllowed)) {
			fromIndex = randomFromIndex;
			toIndex = randomToIndex;
			break;
		}
	}

	if (fromIndex == -1 || toIndex == -1)
		return;

	const bool recurrent = isRecurrentBetweenNeurons(neurons[fromIndex], neurons[toIndex]);
	createLinkWithRandomWeight(innovation, neurons[fromIndex].id, neurons[toIndex].id, recurrent);
}

double Genotype::calculateCompatibilityScore(Genotype& left, Genotype& right, const float& exzessFactor,
	const float& disjointFactor, const float& weightFactor)
{
	sort(right.links.begin(), right.links.end());
	sort(left.links.begin(), left.links.end());
	int countOfExzess = 0;
	int countOfCommon = 0;
	int countOfDisjoint = 0;
	double totalWeightDifference = 0;

	int genotypeIndex = 0;
	int partnerIndex = 0;

	while (genotypeIndex < left.links.size() || partnerIndex < right.links.size()) {
		if (genotypeIndex >= left.links.size()) {
			partnerIndex++;
			countOfExzess++;
		}
		else if (partnerIndex >= right.links.size()) {
			genotypeIndex++;
			countOfExzess++;
		}
		else if (left.links[genotypeIndex].innovationID == right.links[partnerIndex].innovationID) {
			countOfCommon++;
			totalWeightDifference += abs(left.links[genotypeIndex].weight - right.links[partnerIndex].weight);
			genotypeIndex++;
			partnerIndex++;
		}
		else if (left.links[genotypeIndex].innovationID > right.links[partnerIndex].innovationID) {
			countOfDisjoint++;
			partnerIndex++;
		}
		else if (left.links[genotypeIndex].innovationID < right.links[partnerIndex].innovationID) {
			countOfDisjoint++;
			genotypeIndex++;
		}
	}
	int maxCountOfGenes = std::max(left.links.size(), right.links.size());
	if (maxCountOfGenes < 20)
		maxCountOfGenes = 1;


	const double compatibilityScore = (((double)countOfExzess * exzessFactor) / (double)maxCountOfGenes)
		+ (((double)countOfDisjoint * disjointFactor) / (double)maxCountOfGenes)
		+ ((totalWeightDifference / (double)countOfCommon) * weightFactor);

	return compatibilityScore;
}

Genotype Genotype::crossOver(const Genotype& father, const Genotype& mother, const int& babyId)
{
	std::vector<LinkGene> motherLinks = mother.links;
	std::vector<LinkGene> fatherLinks = father.links;
	int fatherLinkIndex = 0;
	int motherLinkIndex = 0;
	sort(fatherLinks.begin(), fatherLinks.end());
	sort(motherLinks.begin(), motherLinks.end());
	std::vector<LinkGene> babyLinks;
	std::vector<NeuronGene> babyNeurons;

	ParentType highestFitness = getFittestParent(father, mother);

	while ((motherLinkIndex < motherLinks.size()) || (fatherLinkIndex < fatherLinks.size())) {
		if (fatherLinkIndex >= fatherLinks.size()) {
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(mother, motherLinkIndex, babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (motherLinkIndex >= motherLinks.size()) {
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(father, fatherLinkIndex, babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (motherLinks[motherLinkIndex].innovationID > fatherLinks[fatherLinkIndex].innovationID) {
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(father, fatherLinkIndex, babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID > motherLinks[motherLinkIndex].innovationID) {
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(mother, motherLinkIndex, babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID == motherLinks[motherLinkIndex].innovationID) {
			double weight = RNG::getRandomIntBetween(0, 1) == 0 ? fatherLinks[fatherLinkIndex].weight : motherLinks[motherLinkIndex].weight;
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(mother, motherLinkIndex, weight, babyNeurons, babyLinks);
			else
				addGeneToVectorIfNotAlreadyInside(father, fatherLinkIndex, weight, babyNeurons, babyLinks);

			motherLinkIndex++;
			fatherLinkIndex++;
		}
	}
	Genotype baby = Genotype(babyNeurons, babyLinks, babyId, father.activationFunction);
	return baby;
}

std::vector<double> Genotype::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputSnapshot(inputs, this->activationFunction);
}

std::vector<double> Genotype::calculateOutputActive(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputActive(inputs, this->activationFunction);
}

void Genotype::createPhenotype()
{
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
		toNeuron->linksIn.push_back(PhenotypeNeuron::Link(fromNeuron, links[i].weight));
	}

	phenotype = std::make_shared<Phenotype>(phenoNeurons, maxDepth);
}

void Genotype::deletePhenotype()
{
	phenotype = nullptr;
}

void Genotype::setAdjustedFitness(const double& fitness)
{
	this->adjustedFitness = fitness;
}

void Genotype::setRawFitness(const double& fitness)
{
	this->rawFitness = fitness;
}

double Genotype::getAdjustedFitness() const
{
	return adjustedFitness;
}

double Genotype::getRawFitness() const
{
	return rawFitness;
}

int Genotype::getCountOfNeurons() const
{
	return neurons.size();
}

int Genotype::getCountOfLinks() const
{
	return links.size();
}

int Genotype::getMaxDepth() const
{
	return maxDepth;
}

double Genotype::getLinkWeightAverage() const
{
	double total = 0;
	if (links.size() == 0)
		return 0;

	for (int i = 0; i < links.size(); i++) {
		total += links[i].weight;
	}
	return total / links.size();
}

int Genotype::getGenotypeId() const
{
	return id;
}

int Genotype::getCountOfInputs() const
{
	return countOfInputs;
}

int Genotype::getCountOfOutputs() const
{
	return countOfOutputs;
}

std::vector<NeuronGene> Genotype::getNeurons() const
{
	return neurons;
}

std::vector<LinkGene> Genotype::getLinks() const
{
	return links;
}

NeuronGene Genotype::getNeuronGeneFromId(const int& id) const
{
	return getNeuronGeneFromId(this->neurons, id);
}

NeuronGene Genotype::getNeuronGeneFromId(const std::vector<NeuronGene>& neurons, const int& id)
{
	int neuronIndex = getNeuronIndexFromId(neurons, id);
	if (neuronIndex != -1)
		return neurons[neuronIndex];

	return NeuronGene();
}

void Genotype::addLinkToVectorIfNotAlreadyInside(const LinkGene& link, std::vector<LinkGene>& linkVec)
{
	for (LinkGene& linkInVec : linkVec) {
		if ((linkInVec.fromNeuronID == link.fromNeuronID) && (linkInVec.toNeuronID == link.toNeuronID))
			return;
	}

	linkVec.push_back(link);
}

void Genotype::addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec)
{
	for (NeuronGene& neuronInVec : neuronVec) {
		if (neuron.id == neuronInVec.id)
			return;
	}

	neuronVec.push_back(neuron);
}

void Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const int& linkIndex, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	addGeneToVectorIfNotAlreadyInside(geno, linkIndex, geno.links[linkIndex].weight, destNeuronVec, destLinkVec);
}

void Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const int& linkIndex, const double& weight, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	LinkGene linkGene = geno.links[linkIndex];
	linkGene.weight = weight;
	NeuronGene fromNeuron = getNeuronGeneFromId(geno.neurons, geno.links[linkIndex].fromNeuronID);
	NeuronGene toNeuron = getNeuronGeneFromId(geno.neurons, geno.links[linkIndex].toNeuronID);

	addLinkToVectorIfNotAlreadyInside(linkGene, destLinkVec);
	addNeuronToVectorIfNotAlreadyInside(fromNeuron, destNeuronVec);
	addNeuronToVectorIfNotAlreadyInside(toNeuron, destNeuronVec);
}

void Genotype::createFullyConnectedNetwork(Innovation& innovation)
{
	if (countOfInputs <= 0 || countOfOutputs <= 0)
		return;

	createNeurons();
	createLinks(innovation);
	calculateDepthOfEveryNeuron();
}

void Genotype::createNeurons()
{
	neurons.push_back(NeuronGene(NeuronType::bias, 0));

	int neuronId = 1;
	for (int i = 0; i < countOfInputs; i++) {
		neurons.push_back(NeuronGene(NeuronType::input, neuronId));
		neuronId++;
	}

	for (int i = 0; i < countOfOutputs; i++) {
		neurons.push_back(NeuronGene(NeuronType::output, neuronId));
		neuronId++;
	}
}

void Genotype::createLinks(Innovation& innovation)
{
	for (int inputNeuronID = 0; inputNeuronID <= countOfInputs; inputNeuronID++) {
		for (int x = 0; x < countOfOutputs; x++) {
			int outputNeuronID = neurons.size() - 1 - x;

			createLinkWithRandomWeight(innovation, inputNeuronID, outputNeuronID, false);
		}
	}
}

void Genotype::calculateDepthOfEveryNeuron()
{
	if (neurons.size() == 0 || countOfInputs == 0 || countOfOutputs == 0)
		return;

	maxDepth = -1;
	for (NeuronGene& neuron : neurons) {
		if (neuron.neuronType == NeuronType::bias || neuron.neuronType == NeuronType::input)
			neuron.depth = 0;
		else
			neuron.depth = -1;
	}

	for (NeuronGene& neuron : neurons) {
		if (neuron.neuronType == NeuronType::bias || neuron.neuronType == NeuronType::input)
			updateDepthOfNeuronsConnectedToThis(neuron);
	}
}

void Genotype::updateDepthOfNeuronsConnectedToThis(NeuronGene& fromNeuron)
{
	for (const LinkGene& link : links) {
		if (!link.enabled || link.recurrent)
			continue;

		if (link.fromNeuronID != fromNeuron.id)
			continue;

		int toIndex = getNeuronIndexFromId(link.toNeuronID);
		if (neurons[toIndex].depth <= fromNeuron.depth) {
			int newDepth = fromNeuron.depth + 1;
			neurons[toIndex].depth = newDepth;

			if (newDepth > maxDepth)
				maxDepth = newDepth;

			updateDepthOfNeuronsConnectedToThis(neurons[toIndex]);
		}
	}
}

bool Genotype::isValidLinkForAddNeuron(const LinkGene& link) const
{
	int fromIndex = getNeuronIndexFromId(link.fromNeuronID);
	if (fromIndex == -1 || !link.enabled || link.recurrent || neurons[fromIndex].neuronType == NeuronType::bias)
		return false;

	return true;
}

bool Genotype::areValidNeuronsForAddLink(const NeuronGene& fromNeuron, const NeuronGene& toNeuron, const bool& recurrentAllowed) const
{
	if (fromNeuron.neuronType == NeuronType::bias && toNeuron.neuronType == NeuronType::input ||
		fromNeuron.neuronType == NeuronType::input && toNeuron.neuronType == NeuronType::bias ||
		fromNeuron.neuronType == NeuronType::bias && toNeuron.neuronType == NeuronType::bias)
		return false;

	if (doesLinkAlreadyExist(fromNeuron, toNeuron))
		return false;

	if (recurrentAllowed)
		return true;
	else if (isRecurrentBetweenNeurons(fromNeuron, toNeuron))
		return false;

	return true;
}

bool Genotype::doesLinkAlreadyExist(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const
{
	int fromId = fromNeuron.id;
	int toId = toNeuron.id;

	for (int i = 0; i < links.size(); i++) {
		if (links[i].fromNeuronID == fromId && links[i].toNeuronID == toId)
			return true;
	}
	return false;
}

bool Genotype::isRecurrentBetweenNeurons(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const
{
	if (fromNeuron.depth >= toNeuron.depth)
		return true;

	return false;
}

int Genotype::getNeuronIndexFromId(const int& id) const
{
	return getNeuronIndexFromId(this->neurons, id);
}

int Genotype::getNeuronIndexFromId(const std::vector<NeuronGene>& neurons, const int& id)
{
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].id == id)
			return i;
	}
	return -1;
}

double Genotype::getRandomLinkWeight() const
{
	return RNG::getRandomDoubleBetween(minimumLinkStartValue, maximumLinkStartValue);
}

void Genotype::createLinkWithRandomWeight(Innovation& innovation, const int& fromId, const int& toId, const bool& recurrent)
{
	createLink(innovation, fromId, toId, recurrent, getRandomLinkWeight());
}

void Genotype::createLink(Innovation& innovation, const int& fromId, const int& toId, const bool& recurrent, const double& weightOfLink)
{
	int innovationId = innovation.getInnovationID(fromId, toId, InnovationType::newLink);

	if (innovationId == -1)
		innovationId = innovation.createNewLinkInnovation(fromId, toId);

	links.push_back(LinkGene(fromId, toId, weightOfLink, true, innovationId, recurrent));
}

bool operator<(const Genotype& lhs, const Genotype& rhs)
{
	return lhs.rawFitness > rhs.rawFitness;
}

ParentType Genotype::getFittestParent(const Genotype& father, const Genotype& mother)
{
	if (mother.rawFitness > father.rawFitness)
		return ParentType::Mother;
	else if (mother.rawFitness == father.rawFitness)
		return RNG::getRandomIntBetween(0, 1) == 0 ? ParentType::Mother : ParentType::Father;

	return ParentType::Father;
}