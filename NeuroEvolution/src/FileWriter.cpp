#include "FileWriter.h"

void nev::FileWriter::writeNEATToFile(const nev::NEAT& neat, const std::string& fileSuffix)
{
	std::string populationFileName = "population" + fileSuffix + ".txt";
	std::string innovationFileName = "innovation" + fileSuffix + ".txt";
	std::string neatFileName = "neat" + fileSuffix + ".txt";

	writePopulationToFile(populationFileName, neat.population);
	writeInnovationToFile(innovationFileName, neat.innovation);
	writeNEATParametersToFile(neatFileName, neat);
}

void nev::FileWriter::writeNEATParametersToFile(const std::string& fileName, const nev::NEAT& neat)
{
	std::ofstream neatFile;
	neatFile.open(fileName);
	neatFile << stringifyNEAT(neat);
	neatFile.close();
}

void nev::FileWriter::writePopulationToFile(const std::string& fileName, std::vector<Genotype> population)
{
	std::ofstream populationFile;
	populationFile.open(fileName);

	sort(population.begin(), population.end());
	for (int i = 0; i < population.size(); i++)
		populationFile << stringifyGenotype(population[i]);
	populationFile.close();
}

void nev::FileWriter::writeInnovationToFile(const std::string& fileName, const Innovation& innovation)
{
	std::ofstream innovationFile;
	innovationFile.open(fileName);
	innovationFile << stringifyInnovation(innovation);
	innovationFile.close();
}

std::string nev::FileWriter::stringifyGenotype(const Genotype& genotype)
{
	std::string genotypeString = "'BeginGenotype'\n";
	genotypeString = genotypeString + "CountOfInputs: '" + std::to_string(genotype.getCountOfInputs()) + "'\n";
	genotypeString = genotypeString + "CountOfOuputs: '" + std::to_string(genotype.getCountOfOutputs()) + "'\n";
	genotypeString = genotypeString + "MaxDepth: '" + std::to_string(genotype.getMaxDepth()) + "'\n \n";
	genotypeString = genotypeString + "BeginNeurons\n";

	std::vector<NeuronGene> neurons = genotype.getNeurons();
	for (int i = 0; i < neurons.size(); i++) {
		genotypeString = genotypeString + "NeuronStart" + "\n";
		genotypeString = genotypeString + "Depth: '" + std::to_string(neurons[i].depth) + "'\n";
		genotypeString = genotypeString + "ID: '" + std::to_string(neurons[i].id) + "'\n";
		genotypeString = genotypeString + "NeuronType: '" + std::to_string((int)neurons[i].neuronType) + "'\n";
		genotypeString = genotypeString + "NeuronEnd" + "\n";
	}
	genotypeString = genotypeString + "EndNeurons\n\n";

	std::vector<LinkGene> links = genotype.getLinks();
	genotypeString = genotypeString + "BeginLinks\n";
	for (int i = 0; i < links.size(); i++) {
		genotypeString = genotypeString + "LinkStart" + "\n";
		genotypeString = genotypeString + "Enabled: '" + std::to_string(links[i].enabled) + "'\n";
		genotypeString = genotypeString + "FromID: '" + std::to_string(links[i].fromNeuronID) + "'\n";
		genotypeString = genotypeString + "InnovationID: '" + std::to_string(links[i].innovationID) + "'\n";
		genotypeString = genotypeString + "Recurrent: '" + std::to_string(links[i].recurrent) + "'\n";
		genotypeString = genotypeString + "ToID: '" + std::to_string(links[i].toNeuronID) + "'\n";
		genotypeString = genotypeString + "Weight: '" + to_string_with_max_precision(links[i].weight) + "'\n";
		genotypeString = genotypeString + "LinkEnd" + "\n";
	}
	genotypeString = genotypeString + "EndLinks\n";

	genotypeString = genotypeString + "EndGenotype\n";
	return genotypeString;
}

std::string nev::FileWriter::stringifyInnovation(const Innovation& innovation)
{
	std::string innovationString = "StartInnovations\n";
	innovationString = innovationString + "CurrentInnovationNumber: '" + std::to_string(innovation.getCurrentInnovationNumber()) + "' \n";
	innovationString = innovationString + "StartNeuronNumber: '" + std::to_string(innovation.getCurrentNeuronId()) + "' \n\n";

	std::vector<InnovationElement> innovations = innovation.getInnovations();
	for (int i = 0; i < innovations.size(); i++) {
		innovationString = innovationString + "InnovationBegin \n";

		innovationString = innovationString + "FromID: '" + std::to_string(innovations[i].fromNeuronId) + "' \n";
		innovationString = innovationString + "InnovationID: '" + std::to_string(innovations[i].innovationId) + "' \n";
		innovationString = innovationString + "innovationType: '" + std::to_string((int)innovations[i].innovationType) + "' \n";
		innovationString = innovationString + "NeuronID: '" + std::to_string(innovations[i].neuronId) + "' \n";
		innovationString = innovationString + "ToID: '" + std::to_string(innovations[i].toNeuronId) + "' \n";


		innovationString = innovationString + "InnovationEnd \n";
	}

	innovationString = innovationString + "EndInnovations\n";
	return innovationString;
}

std::string nev::FileWriter::stringifyNEAT(const NEAT& neat)
{
	std::string neatString;

	//NEAT-Parameters
	neatString += "ActivationFunction: '" + std::to_string((int)neat.activationFunction) + "' \n";
	neatString += "ExcessFactor: '" + to_string_with_max_precision(neat.excessFactor) + "' \n";
	neatString += "DisjointFactor: '" + to_string_with_max_precision(neat.disjointFactor) + "' \n";
	neatString += "WeightFactor: '" + to_string_with_max_precision(neat.weightFactor) + "' \n";
	neatString += "CompatibilityDistanceThreshold: '" + to_string_with_max_precision(neat.compatibilityDistanceThreshold) + "' \n";
	neatString += "GenerationsNoImprovementAllowed: '" + std::to_string(neat.generationsNoImprovementAllowed) + "' \n";
	neatString += "SpeciesRoughValue: '" + std::to_string(neat.speciesRoughValue) + "' \n";
	neatString += "MaxCountSpecies: '" + std::to_string(neat.maxCountSpecies) + "' \n";
	neatString += "RecurrentAllowed: '" + std::to_string(neat.recurrentAllowed) + "' \n";

	//Probabilities
	neatString += "CrossOverProbability: '" + to_string_with_max_precision(neat.crossOverProbability) + "' \n";
	neatString += "AddNeuronProbability: '" + to_string_with_max_precision(neat.addNeuronProbability) + "' \n";
	neatString += "AddLinkProbability: '" + to_string_with_max_precision(neat.addLinkProbability) + "' \n";
	neatString += "MutateLinkProbability: '" + to_string_with_max_precision(neat.mutateLinkProbability) + "' \n";
	neatString += "NewLinkWeightProbability: '" + to_string_with_max_precision(neat.newLinkWeightProbability) + "' \n";
	neatString += "WeightPertubation: '" + to_string_with_max_precision(neat.weightPertubation) + "' \n";

	neatString += "CurrentGeneration: '" + std::to_string(neat.currentGeneration) + "' \n";

	return neatString;
}

template<typename T>
inline std::string nev::FileWriter::to_string_with_max_precision(const T& a_value)
{
	int maxPrecision = std::numeric_limits<T>::max_digits10;

	std::stringstream out;
	out.precision(maxPrecision);
	out << a_value;
	return out.str();
}