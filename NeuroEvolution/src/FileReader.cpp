#include "FileReader.h"


std::vector<Genotype> FileReader::parsePopulationFromFile(string fileName)
{
	std::vector<Genotype> population;
	ifstream populationFile(fileName);
	for (string line; getline(populationFile, line);) {
		if (contains(line, "BeginGenotype")) {
			population.push_back(parseOneGenotype(populationFile));
		}
	}
	return population;
}

Genotype FileReader::parseOneGenotype(ifstream & populationFile)
{
	int countOfInputs = 0;
	int countOfOutputs = 0;
	int maxDepth = 0;
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;

	for (string line; getline(populationFile, line);) {
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

std::vector<NeuronGene> FileReader::parseNeurons(ifstream & populationFile)
{
	std::vector<NeuronGene> neurons;
	bool finishedParsing = false;

	for (string line; getline(populationFile, line) && !finishedParsing;) {
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

NeuronGene FileReader::parseOneNeuron(ifstream & populationFile)
{
	NeuronGene neuron;
	for (string line; getline(populationFile, line);) {
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

std::vector<LinkGene> FileReader::parseLinks(ifstream & populationFile)
{
	std::vector<LinkGene> links;

	for (string line; getline(populationFile, line);) {
		if (contains(line, "EndLinks")) {
			break;
		}

		if (contains(line, "LinkStart")) {
			links.push_back(parseOneLink(populationFile));
		}
	}

	return links;
}

LinkGene FileReader::parseOneLink(ifstream & populationFile)
{
	LinkGene link;
	for (string line; getline(populationFile, line);) {
		if (contains(line, "LinkEnd")) {
			break;
		}

		if (contains(line, "Enabled")) {
			link.enabled = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "FromID")) {
			link.fromID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "InnovationID")) {
			link.innovationID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "Recurrent")) {
			link.recurrent = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "ToID")) {
			link.toID = atoi(getStringBetweenQuotationMark(line).c_str());
		}

		if (contains(line, "Weight")) {
			link.weight = atof(getStringBetweenQuotationMark(line).c_str());
		}
	}
	return link;
}

Innovation FileReader::parseInnovationFromFile(string fileName)
{
	ifstream innovationFile(fileName);
	std::vector<InnovationElement> innovations;
	int currentInnovationNumber = 0;
	int startNeuronNumber = 0;

	for (string line; getline(innovationFile, line);) {
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

InnovationElement FileReader::parseOneInnovation(ifstream & innovationFile)
{
	int fromId = 0;
	int innovationId = 0;
	InnovationType innovationType = newNeuron;
	int neuronId = 0;
	int toId = 0;

	for (string line; getline(innovationFile, line);) {
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

bool FileReader::contains(string line, string searched)
{
	std::size_t found = line.find(searched);
	if (found != std::string::npos) {
		return true;
	}
	return false;
}

string FileReader::getStringBetweenQuotationMark(string line)
{
	string parsed;
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
