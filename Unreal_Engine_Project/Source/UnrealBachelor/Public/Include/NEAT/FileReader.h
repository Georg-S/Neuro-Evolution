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

	static vector<Genotype> parsePopulationFromFile(string fileName);
	static Innovation parseInnovationsFromFile(string fileName);
	static string getStringBetweenQuotationMark(string line);

private:
	static bool contains(string line, string searched);
	static Genotype parseOneGenotype(ifstream &populationFile);
	static vector<NeuronGene> parseNeurons(ifstream &populationFile);
	static NeuronGene parseOneNeuron(ifstream &populationFile);
	static vector<LinkGene> parseLinks(ifstream &populationFile);
	static LinkGene parseOneLink(ifstream &populationFile);
	static InnovationElement parseOneInnovation(ifstream & innovationFile);
};

