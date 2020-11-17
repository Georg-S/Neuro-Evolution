#include "FileReader.h"


std::vector<Genotype> FileReader::parsePopulationFromFile(const std::string& fileName)
{
	std::vector<Genotype> population;
	std::ifstream populationFile(fileName);
	for (std::string line; getline(populationFile, line);) {
		if (contains(line, "BeginGenotype")) {
			population.push_back(parseOneGenotype(populationFile));
		}
	}
	return population;
}

Genotype FileReader::parseOneGenotype(std::ifstream& populationFile)
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
	Genotype createdGenotype = Genotype(neurons, links, 0);
	return createdGenotype;
}

std::vector<NeuronGene> FileReader::parseNeurons(std::ifstream& populationFile)
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

NeuronGene FileReader::parseOneNeuron(std::ifstream& populationFile)
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

std::vector<LinkGene> FileReader::parseLinks(std::ifstream& populationFile)
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

LinkGene FileReader::parseOneLink(std::ifstream& populationFile)
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

Innovation FileReader::parseInnovationFromFile(const std::string& fileName)
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

InnovationElement FileReader::parseOneInnovation(std::ifstream& innovationFile)
{
	int fromId = 0;
	int innovationId = 0;
	InnovationType innovationType = newNeuron;
	int neuronId = 0;
	int toId = 0;

	for (std::string line; getline(innovationFile, line);) {
		if (contains(line, "InnovationEnd")) {
			break;
		}

		if (contains(line, "FromID")) {
			fromId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "InnovationID")) {
			innovationId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "innovationType")) {
			innovationType = (InnovationType)atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "NeuronID")) {
			neuronId = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "ToID")) {
			toId = atoi(getStringBetweenQuotationMark(line).c_str());
		}
	}
	return InnovationElement(fromId, toId, innovationId, neuronId, innovationType);
}

bool FileReader::contains(const std::string& line, const std::string& searched)
{
	std::size_t found = line.find(searched);
	if (found != std::string::npos) {
		return true;
	}
	return false;
}

std::string FileReader::getStringBetweenQuotationMark(const std::string& line)
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
