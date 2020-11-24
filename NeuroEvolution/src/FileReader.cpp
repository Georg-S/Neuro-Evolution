#include "FileReader.h"


nev::NEAT nev::FileReader::getNEATFromFiles(const std::string& neatFile, const std::string& populationFile, const std::string& innovationFile)
{
	
	auto population = parsePopulationFromFile(populationFile);
	auto innovation = parseInnovationFromFile(innovationFile);
	NEAT result = NEAT(population, innovation);
	result.currentGenotypeId = population.size();
	parseNEATParameters(neatFile, result);
	result.refreshPopulationActivationFunction();

	return result;
}

std::vector<nev::Genotype> nev::FileReader::parsePopulationFromFile(const std::string& fileName)
{
	int genotypeID = 0;
	std::vector<Genotype> population;
	std::ifstream populationFile(fileName);
	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "BeginGenotype")) {
			population.push_back(parseOneGenotype(populationFile, genotypeID));
		}
	}
	return population;
}

nev::Genotype nev::FileReader::parseOneGenotype(std::ifstream& populationFile, int& genotypeID)
{
	int countOfInputs = 0;
	int countOfOutputs = 0;
	int maxDepth = 0;
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "EndGenotype")) {
			break;
		}
		if (contains(line, "CountOfInputs")) {
			countOfInputs = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "CountOfOuputs")) {
			countOfOutputs = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "MaxDepth")) {
			maxDepth = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "BeginNeurons")) {
			neurons = parseNeurons(populationFile);
		}

		if (contains(line, "BeginLinks")) {
			links = parseLinks(populationFile);
		}
	}
	return Genotype(neurons, links, genotypeID++);
}

std::vector<nev::NeuronGene> nev::FileReader::parseNeurons(std::ifstream& populationFile)
{
	std::vector<NeuronGene> neurons;
	bool finishedParsing = false;

	for (std::string line; getline(populationFile, line) && !finishedParsing;) {
		if (contains(line, "EndNeurons")) {
			finishedParsing = true;
			break;
		}
		if (contains(line, "NeuronStart")) {
			neurons.push_back(parseOneNeuron(populationFile));
		}
	}
	return neurons;
}

nev::NeuronGene nev::FileReader::parseOneNeuron(std::ifstream& populationFile)
{
	NeuronGene neuron;
	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "NeuronEnd")) {
			break;
		}

		if (contains(line, "Depth")) {
			neuron.depth = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "ID")) {
			neuron.id = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "NeuronType")) {
			neuron.neuronType = (NeuronType)atoi(getStringBetweenQuotationMark(line).c_str());
		}
	}
	return neuron;
}

std::vector<nev::LinkGene> nev::FileReader::parseLinks(std::ifstream& populationFile)
{
	std::vector<LinkGene> links;

	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "EndLinks")) {
			break;
		}

		if (contains(line, "LinkStart")) {
			links.push_back(parseOneLink(populationFile));
		}
	}

	return links;
}

nev::LinkGene nev::FileReader::parseOneLink(std::ifstream& populationFile)
{
	LinkGene link;
	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "LinkEnd")) {
			break;
		}

		if (contains(line, "Enabled")) {
			link.enabled = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "FromID")) {
			link.fromNeuronID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "InnovationID")) {
			link.innovationID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "Recurrent")) {
			link.recurrent = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "ToID")) {
			link.toNeuronID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "Weight")) {
			link.weight = std::stod(getStringBetweenQuotationMark(line).c_str());
		}
	}
	return link;
}

nev::Innovation nev::FileReader::parseInnovationFromFile(const std::string& fileName)
{
	std::ifstream innovationFile(fileName);
	std::vector<InnovationElement> innovations;
	int currentInnovationNumber = 0;
	int startNeuronNumber = 0;

	for (std::string line; getline(innovationFile, line);) {
		if (contains(line, "CurrentInnovationNumber")) {
			currentInnovationNumber = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "StartNeuronNumber")) {
			startNeuronNumber = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "InnovationBegin")) {
			innovations.push_back(parseOneInnovation(innovationFile));
		}
	}
	return Innovation(currentInnovationNumber, startNeuronNumber, innovations);
}

nev::InnovationElement nev::FileReader::parseOneInnovation(std::ifstream& innovationFile)
{
	int fromId = 0;
	int innovationId = 0;
	InnovationType innovationType = InnovationType::newNeuron;
	int neuronId = 0;
	int toId = 0;

	for (std::string line; getline(innovationFile, line);) {
		if (contains(line, "InnovationEnd")) {
			break;
		}

		else if (contains(line, "FromID")) {
			fromId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		else if (contains(line, "InnovationID")) {
			innovationId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		else if (contains(line, "innovationType")) {
			innovationType = (InnovationType)atoi(getStringBetweenQuotationMark(line).c_str());
		}

		else if (contains(line, "NeuronID")) {
			neuronId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		else if (contains(line, "ToID")) {
			toId = atoi(getStringBetweenQuotationMark(line).c_str());
		}
	}
	return InnovationElement(fromId, toId, innovationId, neuronId, innovationType);
}

void nev::FileReader::parseNEATParameters(const std::string& fileName, NEAT& neat)
{
	std::ifstream neatFile(fileName);
	for (std::string line; getline(neatFile, line);) {
		if (contains(line, "ActivationFunction"))
			neat.activationFunction = (nev::af)atoi(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "ExcessFactor"))
			neat.excessFactor = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "DisjointFactor"))
			neat.disjointFactor = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "WeightFactor"))
			neat.weightFactor = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "CompatibilityDistanceThreshold"))
			neat.compatibilityDistanceThreshold = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "GenerationsNoImprovementAllowed"))
			neat.generationsNoImprovementAllowed = std::atoi(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "SpeciesRoughValue"))
			neat.speciesRoughValue = std::atoi(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "MaxCountSpecies"))
			neat.maxCountSpecies = std::atoi(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "RecurrentAllowed"))
			neat.recurrentAllowed = std::atoi(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "CrossOverProbability"))
			neat.crossOverProbability = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "AddNeuronProbability"))
			neat.addNeuronProbability = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "AddLinkProbability"))
			neat.addLinkProbability = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "MutateLinkProbability"))
			neat.mutateLinkProbability = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "NewLinkWeightProbability"))
			neat.newLinkWeightProbability = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "WeightPertubation"))
			neat.weightPertubation = std::stod(getStringBetweenQuotationMark(line).c_str());
		else if (contains(line, "CurrentGeneration"))
			neat.currentGeneration = std::atoi(getStringBetweenQuotationMark(line).c_str());
	}
}

bool nev::FileReader::contains(const std::string& line, const std::string& searched)
{
	std::size_t found = line.find(searched);
	if (found != std::string::npos) {
		return true;
	}
	return false;
}

std::string nev::FileReader::getStringBetweenQuotationMark(const std::string& line)
{
	std::string parsed;
	char quotation(39);
	bool parse = false;

	for (int i = 0; i < line.length(); i++) {
		if (line.at(i) == quotation) {
			parse = !parse;
		}
		else if (parse) {
			parsed += line.at(i);
		}
	}
	return parsed;
}
