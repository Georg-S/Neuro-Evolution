#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Genotype.h"
#include "Innovation.h"

using namespace std;

class FileReader
{
public:
	FileReader();
	~FileReader();

	vector<Genotype> parsePopulationFromFile(string fileName);
	Innovation parseInnovationFromFile(string fileName);
	string getStringBetweenQuotationMark(string line);

private:
	bool contains(string line, string searched);
	Genotype parseOneGenotype(ifstream &populationFile);
	vector<NeuronGene> parseNeurons(ifstream &populationFile);
	NeuronGene parseOneNeuron(ifstream &populationFile);
	vector<LinkGene> parseLinks(ifstream &populationFile);
	LinkGene parseOneLink(ifstream &populationFile);
	InnovationElement parseOneInnovation(ifstream & innovationFile);
};

