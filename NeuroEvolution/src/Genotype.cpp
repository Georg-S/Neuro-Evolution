#include "Genotype.h"


nev::Genotype::Genotype()
{
}

nev::Genotype::Genotype(Innovation& innovation, int countOfInputs, int countOfOutputs,
	int id, const nev::af& activationFunction)
{
	this->activationFunction = activationFunction;
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	this->id = id;
	createFullyConnectedNetwork(innovation);
}

nev::Genotype::Genotype(const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
	int id, const nev::af& activationFunction)
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

nev::Genotype::Genotype(Innovation& innovation, const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
	int id, const nev::af& activationFunction)
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

void nev::Genotype::randomlyAddNeuron(Innovation& innovation, double addNeuronProbability)
{
	if (links.size() == 0)
		return;

	if (RNG::getRandomFloatBetween0and1() > addNeuronProbability)
		return;

	std::vector<LinkGene> possibleLinks = getAllValidLinksForAddNeuron();
	if (possibleLinks.size() == 0)
		return;

	LinkGene link = possibleLinks[RNG::getRandomVectorIndex(possibleLinks.size())];

	int newNeuronId = innovation.getNeuronId(link.fromNeuronID, link.toNeuronID);
	if (newNeuronId == -1)
		newNeuronId = innovation.createNewNeuronInnovation(link.fromNeuronID, link.toNeuronID);
	neurons.push_back(NeuronGene(NeuronType::hidden, newNeuronId));

	createLink(innovation, link.fromNeuronID, newNeuronId, false, 1.0);
	createLink(innovation, newNeuronId, link.toNeuronID, false, link.weight);
	disableLink(link);
	calculateDepthOfEveryNeuron();
}

void nev::Genotype::randomlyMutateAllWeights(double mutationProbability, double newWeightProbability, double weightPertubation)
{
	for (int i = 0; i < links.size(); i++) {
		if (RNG::getRandomFloatBetween0and1() < mutationProbability)
			mutateSingleWeight(links[i], newWeightProbability, weightPertubation);
	}
}

void nev::Genotype::mutateSingleWeight(LinkGene& link, double newWeightProbability, double weightPertubation)
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

void nev::Genotype::randomlyAddLink(Innovation& innovation, double mutationProbability, bool recurrentAllowed)
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

double nev::Genotype::calculateCompatibilityScore(std::shared_ptr<Genotype> left, std::shared_ptr<Genotype> right, double excessFactor,
	double disjointFactor, double weightFactor)
{
	sort(right->links.begin(), right->links.end());
	sort(left->links.begin(), left->links.end());
	double countOfExzess = 0;
	double countOfCommon = 0;
	double countOfDisjoint = 0;
	double totalWeightDifference = 0;

	int genotypeIndex = 0;
	int partnerIndex = 0;

	while (genotypeIndex < left->links.size() || partnerIndex < right->links.size()) {
		if (genotypeIndex >= left->links.size()) {
			partnerIndex++;
			countOfExzess++;
		}
		else if (partnerIndex >= right->links.size()) {
			genotypeIndex++;
			countOfExzess++;
		}
		else if (left->links[genotypeIndex].innovationID == right->links[partnerIndex].innovationID) {
			countOfCommon++;
			totalWeightDifference += abs(left->links[genotypeIndex].weight - right->links[partnerIndex].weight);
			genotypeIndex++;
			partnerIndex++;
		}
		else if (left->links[genotypeIndex].innovationID > right->links[partnerIndex].innovationID) {
			countOfDisjoint++;
			partnerIndex++;
		}
		else if (left->links[genotypeIndex].innovationID < right->links[partnerIndex].innovationID) {
			countOfDisjoint++;
			genotypeIndex++;
		}
	}
	double maxCountOfGenes = std::max(left->links.size(), right->links.size());
	if (maxCountOfGenes < 20)
		maxCountOfGenes = 1;


	const double compatibilityScore = ((countOfExzess * excessFactor) / maxCountOfGenes)
		+ ((countOfDisjoint * disjointFactor) / maxCountOfGenes)
		+ ((totalWeightDifference / countOfCommon) * weightFactor);

	return compatibilityScore;
}

std::shared_ptr<nev::Genotype> nev::Genotype::crossOver(std::shared_ptr<Genotype> father, std::shared_ptr<Genotype> mother, int babyId)
{
	std::vector<LinkGene> motherLinks = mother->links;
	std::vector<LinkGene> fatherLinks = father->links;
	int fatherLinkIndex = 0;
	int motherLinkIndex = 0;
	sort(fatherLinks.begin(), fatherLinks.end());
	sort(motherLinks.begin(), motherLinks.end());
	std::vector<LinkGene> babyLinks;
	std::vector<NeuronGene> babyNeurons;

	ParentType highestFitness = getFittestParent(*father, *mother);

	while ((motherLinkIndex < motherLinks.size()) || (fatherLinkIndex < fatherLinks.size())) {
		if (fatherLinkIndex >= fatherLinks.size()) {
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinkIndex, babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (motherLinkIndex >= motherLinks.size()) {
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinkIndex, babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (motherLinks[motherLinkIndex].innovationID > fatherLinks[fatherLinkIndex].innovationID) {
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinkIndex, babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID > motherLinks[motherLinkIndex].innovationID) {
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinkIndex, babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID == motherLinks[motherLinkIndex].innovationID) {
			double weight = RNG::getRandomIntBetween(0, 1) == 0 ? fatherLinks[fatherLinkIndex].weight : motherLinks[motherLinkIndex].weight;
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinkIndex, weight, babyNeurons, babyLinks);
			else
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinkIndex, weight, babyNeurons, babyLinks);

			motherLinkIndex++;
			fatherLinkIndex++;
		}
	}
	return std::make_shared<Genotype>(babyNeurons, babyLinks, babyId, father->activationFunction);
}

std::vector<double> nev::Genotype::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputSnapshot(inputs, this->activationFunction);
}

std::vector<double> nev::Genotype::calculateOutputActive(const std::vector<double>& inputs)
{
	if (phenotype == nullptr)
		createPhenotype();

	return phenotype->calculateOutputActive(inputs, this->activationFunction);
}

void nev::Genotype::createPhenotype()
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

void nev::Genotype::deletePhenotype()
{
	phenotype = nullptr;
}

void nev::Genotype::setActivationFunction(const nev::af& activationFunction)
{
	this->activationFunction = activationFunction;
}

void nev::Genotype::setAdjustedFitness(double fitness)
{
	this->adjustedFitness = fitness;
}

void nev::Genotype::setRawFitness(double fitness)
{
	this->rawFitness = fitness;
}

double nev::Genotype::getAdjustedFitness() const
{
	return adjustedFitness;
}

double nev::Genotype::getRawFitness() const
{
	return rawFitness;
}

int nev::Genotype::getCountOfNeurons() const
{
	return neurons.size();
}

int nev::Genotype::getCountOfLinks() const
{
	return links.size();
}

int nev::Genotype::getMaxDepth() const
{
	return maxDepth;
}

double nev::Genotype::getLinkWeightAverage() const
{
	double total = 0;
	if (links.size() == 0)
		return 0;

	for (int i = 0; i < links.size(); i++) {
		total += links[i].weight;
	}
	return total / links.size();
}

int nev::Genotype::getGenotypeId() const
{
	return id;
}

int nev::Genotype::getCountOfInputs() const
{
	return countOfInputs;
}

int nev::Genotype::getCountOfOutputs() const
{
	return countOfOutputs;
}

int nev::Genotype::getCountOfEnabledLink() const
{
	int count = 0;
	for (auto link : links) {
		if (link.enabled)
			count++;
	}
	return count;
}

int nev::Genotype::getCountOfHiddenNeurons() const
{
	int count = 0;
	for (auto neuron : neurons) {
		if (neuron.neuronType == nev::NeuronType::hidden)
			count++;
	}
	return count;
}

std::vector<nev::NeuronGene> nev::Genotype::getNeurons() const
{
	return neurons;
}

std::vector<nev::LinkGene> nev::Genotype::getLinks() const
{
	return links;
}

nev::NeuronGene nev::Genotype::getNeuronGeneFromId(int id) const
{
	return getNeuronGeneFromId(this->neurons, id);
}

nev::NeuronGene nev::Genotype::getNeuronGeneFromId(const std::vector<NeuronGene>& neurons, int id)
{
	int neuronIndex = getNeuronIndexFromId(neurons, id);
	if (neuronIndex != -1)
		return neurons[neuronIndex];

	return NeuronGene();
}

void nev::Genotype::addLinkToVectorIfNotAlreadyInside(const LinkGene& link, std::vector<LinkGene>& linkVec)
{
	for (LinkGene& linkInVec : linkVec) {
		if ((linkInVec.fromNeuronID == link.fromNeuronID) && (linkInVec.toNeuronID == link.toNeuronID))
			return;
	}

	linkVec.push_back(link);
}

void nev::Genotype::addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec)
{
	for (NeuronGene& neuronInVec : neuronVec) {
		if (neuron.id == neuronInVec.id)
			return;
	}

	neuronVec.push_back(neuron);
}

void nev::Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, int linkIndex, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	addGeneToVectorIfNotAlreadyInside(geno, linkIndex, geno.links[linkIndex].weight, destNeuronVec, destLinkVec);
}

void nev::Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, int linkIndex, double weight, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	LinkGene linkGene = geno.links[linkIndex];
	linkGene.weight = weight;
	NeuronGene fromNeuron = getNeuronGeneFromId(geno.neurons, geno.links[linkIndex].fromNeuronID);
	NeuronGene toNeuron = getNeuronGeneFromId(geno.neurons, geno.links[linkIndex].toNeuronID);

	addLinkToVectorIfNotAlreadyInside(linkGene, destLinkVec);
	addNeuronToVectorIfNotAlreadyInside(fromNeuron, destNeuronVec);
	addNeuronToVectorIfNotAlreadyInside(toNeuron, destNeuronVec);
}

void nev::Genotype::createFullyConnectedNetwork(Innovation& innovation)
{
	if (countOfInputs <= 0 || countOfOutputs <= 0)
		return;

	createNeurons();
	createLinks(innovation);
	calculateDepthOfEveryNeuron();
}

void nev::Genotype::createNeurons()
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

void nev::Genotype::createLinks(Innovation& innovation)
{
	for (int inputNeuronID = 0; inputNeuronID <= countOfInputs; inputNeuronID++) {
		for (int x = 0; x < countOfOutputs; x++) {
			int outputNeuronID = neurons.size() - 1 - x;

			createLinkWithRandomWeight(innovation, inputNeuronID, outputNeuronID, false);
		}
	}
}

void nev::Genotype::calculateDepthOfEveryNeuron()
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

void nev::Genotype::updateDepthOfNeuronsConnectedToThis(NeuronGene& fromNeuron)
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

bool nev::Genotype::isValidLinkForAddNeuron(const LinkGene& link) const
{
	int fromIndex = getNeuronIndexFromId(link.fromNeuronID);
	if (fromIndex == -1 || !link.enabled || link.recurrent || neurons[fromIndex].neuronType == NeuronType::bias)
		return false;

	return true;
}

bool nev::Genotype::areValidNeuronsForAddLink(const NeuronGene& fromNeuron, const NeuronGene& toNeuron, bool recurrentAllowed) const
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

bool nev::Genotype::doesLinkAlreadyExist(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const
{
	int fromId = fromNeuron.id;
	int toId = toNeuron.id;

	for (int i = 0; i < links.size(); i++) {
		if (links[i].fromNeuronID == fromId && links[i].toNeuronID == toId)
			return true;
	}
	return false;
}

bool nev::Genotype::isRecurrentBetweenNeurons(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const
{
	if (fromNeuron.depth >= toNeuron.depth)
		return true;

	return false;
}

int nev::Genotype::getNeuronIndexFromId(int id) const
{
	return getNeuronIndexFromId(this->neurons, id);
}

int nev::Genotype::getNeuronIndexFromId(const std::vector<NeuronGene>& neurons, int id)
{
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i].id == id)
			return i;
	}
	return -1;
}

double nev::Genotype::getRandomLinkWeight() const
{
	return RNG::getRandomDoubleBetween(minimumLinkStartValue, maximumLinkStartValue);
}

void nev::Genotype::createLinkWithRandomWeight(Innovation& innovation, int fromId, int toId, bool recurrent)
{
	createLink(innovation, fromId, toId, recurrent, getRandomLinkWeight());
}

void nev::Genotype::createLink(Innovation& innovation, int fromId, int toId, bool recurrent, double weightOfLink)
{
	int innovationId = innovation.getInnovationID(fromId, toId, InnovationType::newLink);

	if (innovationId == -1)
		innovationId = innovation.createNewLinkInnovation(fromId, toId);

	links.push_back(LinkGene(fromId, toId, weightOfLink, true, innovationId, recurrent));
}

std::vector<nev::LinkGene> nev::Genotype::getAllValidLinksForAddNeuron()
{
	std::vector<LinkGene> result;
	for (const LinkGene& link : links) {
		if (isValidLinkForAddNeuron(link))
			result.push_back(link);
	}

	return result;
}

void nev::Genotype::disableLink(const LinkGene& link)
{
	for (int i = 0; i < links.size(); i++) {
		if ((links[i].fromNeuronID == link.fromNeuronID) && (links[i].toNeuronID == link.toNeuronID)) {
			links[i].enabled = false;
			return;
		}
	}
}

bool nev::operator<(const Genotype& lhs, const Genotype& rhs)
{
	return lhs.rawFitness > rhs.rawFitness;
}

nev::ParentType nev::Genotype::getFittestParent(const Genotype& father, const Genotype& mother)
{
	if (mother.rawFitness > father.rawFitness)
		return ParentType::Mother;
	else if (mother.rawFitness == father.rawFitness)
		return RNG::getRandomIntBetween(0, 1) == 0 ? ParentType::Mother : ParentType::Father;

	return ParentType::Father;
}