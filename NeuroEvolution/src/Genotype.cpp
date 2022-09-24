#include "Genotype.h"

nev::Genotype::Genotype(Innovation* innovation, int countOfInputs, int countOfOutputs,
	int id, nev::af activationFunction)
	: m_countOfInputs(countOfInputs)
	, m_countOfOutputs(countOfOutputs)
	, m_id(id)
	, m_activationFunction(activationFunction)
{
	createFullyConnectedNetwork(innovation);
}

// TODO make vectors R value references
nev::Genotype::Genotype(const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
	int id, nev::af activationFunction)
	: m_neurons(neurons)
	, m_links(links)
	, m_id(id)
	, m_activationFunction(activationFunction)
{
	m_countOfInputs = 0;
	m_countOfOutputs = 0;

	for (const auto& neuron : m_neurons)
	{
		if (neuron.neuronType == NeuronType::input)
			m_countOfInputs++;

		if (neuron.neuronType == NeuronType::output)
			m_countOfOutputs++;
	}

	calculateDepthOfEveryNeuron();
}

nev::Genotype::Genotype(Innovation* innovation, const std::vector<NeuronGene>& neurons, const std::vector<LinkGene>& links,
	int id, nev::af activationFunction)
	: m_neurons(neurons)
	, m_links(links)
	, m_id(id)
	, m_activationFunction(activationFunction)
{
	m_countOfInputs = 0;
	m_countOfOutputs = 0;

	int highestNeuronId = 0;
	for (const auto& neuron : m_neurons)
	{
		if (neuron.neuronType == NeuronType::input)
			this->m_countOfInputs++;

		if (neuron.neuronType == NeuronType::output)
			this->m_countOfOutputs++;

		if (neuron.id > highestNeuronId)
			highestNeuronId = neuron.id;
	}

	innovation->setCurrentNeuronId(highestNeuronId);
	calculateDepthOfEveryNeuron();
}

void nev::Genotype::randomlyAddNeuron(Innovation* innovation, double addNeuronProbability)
{
	if (m_links.empty())
		return;

	if (getRandomDouble(0,1) > addNeuronProbability)
		return;

	std::vector<LinkGene> possibleLinks = getAllValidLinksForAddNeuron();
	if (possibleLinks.empty())
		return;

	LinkGene link = getRandomElement(possibleLinks);

	int newNeuronId = innovation->getNeuronId(link.fromNeuronID, link.toNeuronID);
	if (newNeuronId == -1)
		newNeuronId = innovation->createNewNeuronInnovation(link.fromNeuronID, link.toNeuronID);
	m_neurons.emplace_back(NeuronType::hidden, newNeuronId);

	createLink(innovation, link.fromNeuronID, newNeuronId, false, 1.0);
	createLink(innovation, newNeuronId, link.toNeuronID, false, link.weight);
	disableLink(link);
	calculateDepthOfEveryNeuron();
}

void nev::Genotype::randomlyMutateAllWeights(double mutationProbability, double newWeightProbability, double weightPertubation)
{
	for (auto& link : m_links) 
	{
		if (getRandomDouble(0, 1) <= mutationProbability)
			mutateSingleWeight(&link, newWeightProbability, weightPertubation);
	}
}

void nev::Genotype::mutateSingleWeight(LinkGene* link, double newWeightProbability, double weightPertubation)
{
	if (getRandomDouble(0, 1) <= newWeightProbability)
	{
		link->weight = getRandomLinkWeight();
		return;
	}

	double pertubationAmount = weightPertubation;
	if (getRandomBool())
		pertubationAmount *= -1;

	link->weight += pertubationAmount;
	link->weight = std::clamp(link->weight, MinimumLinkWeight, MaximumLinkWeight);
}

void nev::Genotype::randomlyAddLink(Innovation* innovation, double mutationProbability, bool recurrentAllowed)
{
	if (m_neurons.empty())
		return;

	if (getRandomDouble(0, 1) > mutationProbability)
		return;

	int fromIndex = -1;
	int toIndex = -1;

	for (int triesToFindValidIndices = 0; triesToFindValidIndices < NumTriesToAddLink; triesToFindValidIndices++)
	{
		int randomFromIndex = getRandomIndex(m_neurons);
		int randomToIndex = getRandomIndex(m_neurons);

		if (areValidNeuronsForAddLink(m_neurons[randomFromIndex], m_neurons[randomToIndex], recurrentAllowed))
		{
			fromIndex = randomFromIndex;
			toIndex = randomToIndex;
			break;
		}
	}

	if (fromIndex == -1 || toIndex == -1)
		return;

	const bool recurrent = isRecurrentBetweenNeurons(m_neurons[fromIndex], m_neurons[toIndex]);
	createLinkWithRandomWeight(innovation, m_neurons[fromIndex].id, m_neurons[toIndex].id, recurrent);
}

double nev::Genotype::calculateCompatibilityScore(Genotype* left, Genotype* right, double excessFactor, double disjointFactor, double weightFactor)
{
	// TODO maybe think about if the sorting should happen elsewhere
	sort(right->m_links.begin(), right->m_links.end());
	sort(left->m_links.begin(), left->m_links.end());
	double countOfCommon = 0;
	double countOfDisjoint = 0;
	const double countOfExzess = fabs(static_cast<int>(left->m_links.size() - right->m_links.size())); // cast is needed because size - size = unsigned integer
	double totalWeightDifference = 0;

	int genotypeIndex = 0;
	int partnerIndex = 0;

	while (genotypeIndex < left->m_links.size() && partnerIndex < right->m_links.size())
	{
		if (left->m_links[genotypeIndex].innovationID == right->m_links[partnerIndex].innovationID)
		{
			countOfCommon++;
			totalWeightDifference += abs(left->m_links[genotypeIndex].weight - right->m_links[partnerIndex].weight);
			genotypeIndex++;
			partnerIndex++;
		}
		else if (left->m_links[genotypeIndex].innovationID > right->m_links[partnerIndex].innovationID)
		{
			countOfDisjoint++;
			partnerIndex++;
		}
		else if (left->m_links[genotypeIndex].innovationID < right->m_links[partnerIndex].innovationID)
		{
			countOfDisjoint++;
			genotypeIndex++;
		}
	}
	double maxCountOfGenes = std::max(left->m_links.size(), right->m_links.size());
	if (maxCountOfGenes < 20)
		maxCountOfGenes = 1;

	const double compatibilityScore = ((countOfExzess * excessFactor) / maxCountOfGenes)
		+ ((countOfDisjoint * disjointFactor) / maxCountOfGenes)
		+ ((totalWeightDifference / countOfCommon) * weightFactor);

	return compatibilityScore;
}

std::shared_ptr<nev::Genotype> nev::Genotype::crossOver(Genotype* father, Genotype* mother, int babyId)
{
	std::vector<LinkGene>& motherLinks = mother->m_links;
	std::vector<LinkGene>& fatherLinks = father->m_links;
	int fatherLinkIndex = 0;
	int motherLinkIndex = 0;
	sort(fatherLinks.begin(), fatherLinks.end());
	sort(motherLinks.begin(), motherLinks.end());
	std::vector<LinkGene> babyLinks;
	std::vector<NeuronGene> babyNeurons;

	ParentType highestFitness = getFittestParent(*father, *mother);

	while ((motherLinkIndex < motherLinks.size()) || (fatherLinkIndex < fatherLinks.size()))
	{
		if (fatherLinkIndex >= fatherLinks.size())
		{
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinks[motherLinkIndex], babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (motherLinkIndex >= motherLinks.size())
		{
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinks[fatherLinkIndex], babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (motherLinks[motherLinkIndex].innovationID > fatherLinks[fatherLinkIndex].innovationID)
		{
			if (highestFitness == ParentType::Father)
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinks[fatherLinkIndex], babyNeurons, babyLinks);
			fatherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID > motherLinks[motherLinkIndex].innovationID)
		{
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinks[motherLinkIndex], babyNeurons, babyLinks);
			motherLinkIndex++;
		}
		else if (fatherLinks[fatherLinkIndex].innovationID == motherLinks[motherLinkIndex].innovationID)
		{
			double weight = getRandomBool() ? fatherLinks[fatherLinkIndex].weight : motherLinks[motherLinkIndex].weight;
			if (highestFitness == ParentType::Mother)
				addGeneToVectorIfNotAlreadyInside(*mother, motherLinks[motherLinkIndex], weight, babyNeurons, babyLinks);
			else
				addGeneToVectorIfNotAlreadyInside(*father, fatherLinks[fatherLinkIndex], weight, babyNeurons, babyLinks);

			motherLinkIndex++;
			fatherLinkIndex++;
		}
	}
	return std::make_shared<Genotype>(babyNeurons, babyLinks, babyId, father->m_activationFunction);
}

std::vector<double> nev::Genotype::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (!phenotype)
		createPhenotype();

	return phenotype->calculateOutputSnapshot(inputs, m_activationFunction);
}

std::vector<double> nev::Genotype::calculateOutputActive(const std::vector<double>& inputs)
{
	if (!phenotype)
		createPhenotype();

	return phenotype->calculateOutputActive(inputs, m_activationFunction);
}

void nev::Genotype::createPhenotype()
{
	std::vector<std::unique_ptr<PhenotypeNeuron>> phenoNeurons;
	for (const auto& neuron : m_neurons) 
		phenoNeurons.emplace_back(std::make_unique<PhenotypeNeuron>(neuron.neuronType, neuron.id));

	for (const auto& link : m_links) 
	{
		if (!link.enabled)
			continue;

		int fromIndex = getNeuronIndexFromId(link.fromNeuronID);
		int toIndex = getNeuronIndexFromId(link.toNeuronID);
		PhenotypeNeuron* fromNeuron = phenoNeurons[fromIndex].get();
		PhenotypeNeuron* toNeuron = phenoNeurons[toIndex].get();
		toNeuron->linksIn.emplace_back(fromNeuron, link.weight);
	}

	phenotype = std::make_shared<Phenotype>(std::move(phenoNeurons), m_maxDepth);
}

void nev::Genotype::deletePhenotype()
{
	phenotype = nullptr;
}

void nev::Genotype::setActivationFunction(nev::af activationFunction)
{
	m_activationFunction = activationFunction;
}

void nev::Genotype::setAdjustedFitness(double fitness)
{
	m_adjustedFitness = fitness;
}

void nev::Genotype::setRawFitness(double fitness)
{
	m_rawFitness = fitness;
}

double nev::Genotype::getAdjustedFitness() const
{
	return m_adjustedFitness;
}

double nev::Genotype::getRawFitness() const
{
	return m_rawFitness;
}

int nev::Genotype::getCountOfNeurons() const
{
	return m_neurons.size();
}

int nev::Genotype::getCountOfLinks() const
{
	return m_links.size();
}

int nev::Genotype::getMaxDepth() const
{
	return m_maxDepth;
}

double nev::Genotype::getLinkWeightAverage() const
{
	if (m_links.empty())
		return 0.0;

	double total = 0.0;
	for (const auto& link : m_links)
		total += link.weight;

	return total / m_links.size();
}

int nev::Genotype::getGenotypeId() const
{
	return m_id;
}

int nev::Genotype::getCountOfInputs() const
{
	return m_countOfInputs;
}

int nev::Genotype::getCountOfOutputs() const
{
	return m_countOfOutputs;
}

int nev::Genotype::getCountOfEnabledLink() const
{
	int count = 0;
	for (const auto& link : m_links)
	{
		if (link.enabled)
			count++;
	}
	return count;
}

int nev::Genotype::getCountOfHiddenNeurons() const
{
	int count = 0;
	for (const auto& neuron : m_neurons)
	{
		if (neuron.neuronType == nev::NeuronType::hidden)
			count++;
	}
	return count;
}

std::vector<nev::NeuronGene> nev::Genotype::getNeurons() const
{
	return m_neurons;
}

std::vector<nev::LinkGene> nev::Genotype::getLinks() const
{
	return m_links;
}

nev::NeuronGene nev::Genotype::getNeuronGeneFromId(int id) const
{
	return getNeuronGeneFromId(m_neurons, id);
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
	for (const LinkGene& linkInVec : linkVec)
	{
		if ((linkInVec.fromNeuronID == link.fromNeuronID) && (linkInVec.toNeuronID == link.toNeuronID))
			return;
	}

	linkVec.emplace_back(link);
}

void nev::Genotype::addNeuronToVectorIfNotAlreadyInside(const NeuronGene& neuron, std::vector<NeuronGene>& neuronVec)
{
	for (const NeuronGene& neuronInVec : neuronVec)
	{
		if (neuron.id == neuronInVec.id)
			return;
	}

	neuronVec.emplace_back(neuron);
}

void nev::Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const LinkGene& link, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	addGeneToVectorIfNotAlreadyInside(geno, link, link.weight, destNeuronVec, destLinkVec);
}

void nev::Genotype::addGeneToVectorIfNotAlreadyInside(const Genotype& geno, const LinkGene& link, double weight, std::vector<NeuronGene>& destNeuronVec, std::vector<LinkGene>& destLinkVec)
{
	NeuronGene fromNeuron = getNeuronGeneFromId(geno.m_neurons, link.fromNeuronID);
	NeuronGene toNeuron = getNeuronGeneFromId(geno.m_neurons, link.toNeuronID);
	LinkGene linkGene = link;
	linkGene.weight = weight;

	addLinkToVectorIfNotAlreadyInside(linkGene, destLinkVec);
	addNeuronToVectorIfNotAlreadyInside(fromNeuron, destNeuronVec);
	addNeuronToVectorIfNotAlreadyInside(toNeuron, destNeuronVec);
}

void nev::Genotype::createFullyConnectedNetwork(Innovation* innovation)
{
	if (m_countOfInputs <= 0 || m_countOfOutputs <= 0)
		return;

	createNeurons();
	createLinks(innovation);
	calculateDepthOfEveryNeuron();
}

void nev::Genotype::createNeurons()
{
	m_neurons.emplace_back(NeuronType::bias, 0);

	int neuronId = 1;
	for (int i = 0; i < m_countOfInputs; i++)
	{
		m_neurons.emplace_back(NeuronType::input, neuronId);
		neuronId++;
	}

	for (int i = 0; i < m_countOfOutputs; i++)
	{
		m_neurons.emplace_back(NeuronType::output, neuronId);
		neuronId++;
	}
}

void nev::Genotype::createLinks(Innovation* innovation)
{
	for (int inputNeuronID = 0; inputNeuronID <= m_countOfInputs; inputNeuronID++)
	{
		for (int x = 0; x < m_countOfOutputs; x++)
		{
			int outputNeuronID = m_neurons.size() - 1 - x;

			createLinkWithRandomWeight(innovation, inputNeuronID, outputNeuronID, false);
		}
	}
}

void nev::Genotype::calculateDepthOfEveryNeuron()
{
	if (m_neurons.empty() || m_countOfInputs == 0 || m_countOfOutputs == 0)
		return;

	m_maxDepth = -1;
	for (NeuronGene& neuron : m_neurons)
	{
		if (neuron.neuronType == NeuronType::bias || neuron.neuronType == NeuronType::input)
			neuron.depth = 0;
		else
			neuron.depth = -1;
	}

	for (const NeuronGene& neuron : m_neurons)
	{
		if (neuron.neuronType == NeuronType::bias || neuron.neuronType == NeuronType::input)
			updateDepthOfNeuronsConnectedToThis(neuron);
	}
}

void nev::Genotype::updateDepthOfNeuronsConnectedToThis(const NeuronGene& fromNeuron)
{
	for (const LinkGene& link : m_links)
	{
		if (!link.enabled || link.recurrent)
			continue;

		if (link.fromNeuronID != fromNeuron.id)
			continue;

		int toIndex = getNeuronIndexFromId(link.toNeuronID);
		if (m_neurons[toIndex].depth <= fromNeuron.depth)
		{
			int newDepth = fromNeuron.depth + 1;
			m_neurons[toIndex].depth = newDepth;

			if (newDepth > m_maxDepth)
				m_maxDepth = newDepth;

			updateDepthOfNeuronsConnectedToThis(m_neurons[toIndex]);
		}
	}
}

bool nev::Genotype::isValidLinkForAddNeuron(const LinkGene& link) const
{
	int fromIndex = getNeuronIndexFromId(link.fromNeuronID);
	if (fromIndex == -1 || !link.enabled || link.recurrent || m_neurons[fromIndex].neuronType == NeuronType::bias)
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

	if (isRecurrentBetweenNeurons(fromNeuron, toNeuron))
		return false;

	return true;
}

bool nev::Genotype::doesLinkAlreadyExist(const NeuronGene& fromNeuron, const NeuronGene& toNeuron) const
{
	int fromId = fromNeuron.id;
	int toId = toNeuron.id;

	for (const auto& link : m_links)
	{
		if ((link.fromNeuronID == fromId) && (link.toNeuronID == toId))
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
	return getNeuronIndexFromId(this->m_neurons, id);
}

int nev::Genotype::getNeuronIndexFromId(const std::vector<NeuronGene>& neurons, int id)
{
	for (int i = 0; i < neurons.size(); i++)
	{
		if (neurons[i].id == id)
			return i;
	}
	return -1;
}

double nev::Genotype::getRandomLinkWeight() const
{
	return getRandomDouble(MinimumLinkStartValue, MaximumLinkStartValue);
}

void nev::Genotype::createLinkWithRandomWeight(Innovation* innovation, int fromId, int toId, bool recurrent)
{
	createLink(innovation, fromId, toId, recurrent, getRandomLinkWeight());
}

void nev::Genotype::createLink(Innovation* innovation, int fromId, int toId, bool recurrent, double weightOfLink)
{
	int innovationId = innovation->getInnovationID(fromId, toId, InnovationType::newLink);

	if (innovationId == -1)
		innovationId = innovation->createNewLinkInnovation(fromId, toId);

	m_links.emplace_back(fromId, toId, weightOfLink, true, innovationId, recurrent);
}

std::vector<nev::LinkGene> nev::Genotype::getAllValidLinksForAddNeuron()
{
	std::vector<LinkGene> result;
	for (const LinkGene& link : m_links)
	{
		if (isValidLinkForAddNeuron(link))
			result.emplace_back(link);
	}

	return result;
}

void nev::Genotype::disableLink(const LinkGene& linkToDisable)
{
	for (auto& link : m_links)
	{
		if ((link.fromNeuronID == linkToDisable.fromNeuronID) && (link.toNeuronID == linkToDisable.toNeuronID))
		{
			link.enabled = false;
			return;
		}
	}
}

bool nev::operator<(const Genotype& lhs, const Genotype& rhs)
{
	return lhs.m_rawFitness > rhs.m_rawFitness;
}

nev::Genotype::ParentType nev::Genotype::getFittestParent(const Genotype& father, const Genotype& mother)
{
	if (mother.m_rawFitness > father.m_rawFitness)
		return ParentType::Mother;
	if (mother.m_rawFitness == father.m_rawFitness)
		return getRandomBool() ? ParentType::Mother : ParentType::Father;

	return ParentType::Father;
}