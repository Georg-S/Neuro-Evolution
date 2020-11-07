#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Genotype.h"
#include "Innovation.h"


class FileReader
{
public:
	FileReader();
	~FileReader();

	std::vector<Genotype> parsePopulationFromFile(string fileName);
	Innovation parseInnovationFromFile(string fileName);
	string getStringBetweenQuotationMark(string line);

private:
	bool contains(string line, string searched);
	Genotype parseOneGenotype(ifstream &populationFile);
	std::vector<NeuronGene> parseNeurons(ifstream &populationFile);
	NeuronGene parseOneNeuron(ifstream &populationFile);
	std::vector<LinkGene> parseLinks(ifstream &populationFile);
	LinkGene parseOneLink(ifstream &populationFile);
	InnovationElement parseOneInnovation(ifstream & innovationFile);
};

