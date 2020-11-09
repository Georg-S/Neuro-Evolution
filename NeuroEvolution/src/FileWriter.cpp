#include "FileWriter.h"

void FileWriter::writePopulationToFile(string fileName, std::vector<Genotype> population)
{
	ofstream populationFile;
	populationFile.open(fileName);

	sort(population.begin(), population.end());
	for (int i = 0; i < population.size(); i++) {
		//population[i].updateDepthOfNeurons();
		populationFile << stringifyGenotype(population[i]);
	}
	populationFile.close();
}

void FileWriter::writeInnovationToFile(string fileName, Innovation &innovation)
{
	ofstream innovationFile;
	innovationFile.open(fileName);
	innovationFile << stringifyInnovation(innovation);
	innovationFile.close();
}

string FileWriter::stringifyGenotype(Genotype &genotype)
{
	string genotypeString = "'BeginGenotype'\n";
	genotypeString = genotypeString + "CountOfInputs: '" + to_string(genotype.getCountOfInputs()) + "'\n";
	genotypeString = genotypeString + "CountOfOuputs: '" + to_string(genotype.getCountOfOutputs()) + "'\n";
	genotypeString = genotypeString + "MaxDepth: '" + to_string(genotype.getMaxDepth()) + "'\n \n";
	genotypeString = genotypeString + "BeginNeurons\n";

	std::vector<NeuronGene> neurons = genotype.getNeurons();
	for (int i = 0; i < neurons.size(); i++) {
		genotypeString = genotypeString + "NeuronStart" + "\n";
		genotypeString = genotypeString + "Depth: '" + to_string(neurons[i].depth) + "'\n";
		genotypeString = genotypeString + "ID: '" + to_string(neurons[i].id) + "'\n";
		genotypeString = genotypeString + "NeuronType: '" + to_string(neurons[i].neuronType) + "'\n";
		genotypeString = genotypeString + "NeuronEnd" + "\n";
	}
	genotypeString = genotypeString + "EndNeurons\n\n";

	std::vector<LinkGene> links = genotype.getLinks();
	genotypeString = genotypeString + "BeginLinks\n";
	for (int i = 0; i < links.size(); i++) {
		genotypeString = genotypeString + "LinkStart" + "\n";
		genotypeString = genotypeString + "Enabled: '" + to_string(links[i].enabled) + "'\n";
		genotypeString = genotypeString + "FromID: '" + to_string(links[i].fromID) + "'\n";
		genotypeString = genotypeString + "InnovationID: '" + to_string(links[i].innovationID) + "'\n";
		genotypeString = genotypeString + "Recurrent: '" + to_string(links[i].recurrent) + "'\n";
		genotypeString = genotypeString + "ToID: '" + to_string(links[i].toID) + "'\n";
		genotypeString = genotypeString + "Weight: '" + to_string(links[i].weight) + "'\n";
		genotypeString = genotypeString + "LinkEnd" + "\n";
	}
	genotypeString = genotypeString + "EndLinks\n";

	genotypeString = genotypeString + "EndGenotype\n";
	return genotypeString;
}

string FileWriter::stringifyInnovation(Innovation &innovation)
{
	string innovationString = "StartInnovations\n";
	innovationString = innovationString + "CurrentInnovationNumber: '" + to_string(innovation.getCurrentInnovationNumber()) + "' \n";
	innovationString = innovationString + "StartNeuronNumber: '" + to_string(innovation.getCurrentNeuronId()) + "' \n\n";

	std::vector<InnovationElement> innovations = innovation.getInnovations();
	for (int i = 0; i < innovations.size(); i++) {
		innovationString = innovationString + "InnovationBegin \n";

		innovationString = innovationString + "FromID: '" + to_string(innovations[i].fromNeuronId) + "' \n";
		innovationString = innovationString + "InnovationID: '" + to_string(innovations[i].innovationId) + "' \n";
		innovationString = innovationString + "innovationType: '" + to_string(innovations[i].innovationType) + "' \n";
		innovationString = innovationString + "NeuronID: '" + to_string(innovations[i].neuronId) + "' \n";
		innovationString = innovationString + "ToID: '" + to_string(innovations[i].toNeuronId) + "' \n";


		innovationString = innovationString + "InnovationEnd \n";
	}

	innovationString = innovationString + "EndInnovations\n";
	return innovationString;
}

