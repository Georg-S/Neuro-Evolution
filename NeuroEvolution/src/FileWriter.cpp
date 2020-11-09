#include "FileWriter.h"

void FileWriter::writePopulationToFile(std::string fileName, std::vector<Genotype> population)
{
	std::ofstream populationFile;
	populationFile.open(fileName);

	sort(population.begin(), population.end());
	for (int i = 0; i < population.size(); i++) {
		//population[i].updateDepthOfNeurons();
		populationFile << stringifyGenotype(population[i]);
	}
	populationFile.close();
}

void FileWriter::writeInnovationToFile(std::string fileName, Innovation &innovation)
{
	std::ofstream innovationFile;
	innovationFile.open(fileName);
	innovationFile << stringifyInnovation(innovation);
	innovationFile.close();
}

std::string FileWriter::stringifyGenotype(Genotype &genotype)
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
		genotypeString = genotypeString + "NeuronType: '" + std::to_string(neurons[i].neuronType) + "'\n";
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
		genotypeString = genotypeString + "Weight: '" + std::to_string(links[i].weight) + "'\n";
		genotypeString = genotypeString + "LinkEnd" + "\n";
	}
	genotypeString = genotypeString + "EndLinks\n";

	genotypeString = genotypeString + "EndGenotype\n";
	return genotypeString;
}

std::string FileWriter::stringifyInnovation(Innovation &innovation)
{
	std::string innovationString = "StartInnovations\n";
	innovationString = innovationString + "CurrentInnovationNumber: '" + std::to_string(innovation.getCurrentInnovationNumber()) + "' \n";
	innovationString = innovationString + "StartNeuronNumber: '" + std::to_string(innovation.getCurrentNeuronId()) + "' \n\n";

	std::vector<InnovationElement> innovations = innovation.getInnovations();
	for (int i = 0; i < innovations.size(); i++) {
		innovationString = innovationString + "InnovationBegin \n";

		innovationString = innovationString + "FromID: '" + std::to_string(innovations[i].fromNeuronId) + "' \n";
		innovationString = innovationString + "InnovationID: '" + std::to_string(innovations[i].innovationId) + "' \n";
		innovationString = innovationString + "innovationType: '" + std::to_string(innovations[i].innovationType) + "' \n";
		innovationString = innovationString + "NeuronID: '" + std::to_string(innovations[i].neuronId) + "' \n";
		innovationString = innovationString + "ToID: '" + std::to_string(innovations[i].toNeuronId) + "' \n";


		innovationString = innovationString + "InnovationEnd \n";
	}

	innovationString = innovationString + "EndInnovations\n";
	return innovationString;
}

