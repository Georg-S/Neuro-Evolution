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
	static std::vector<Genotype> parsePopulationFromFile(string fileName);
	static Innovation parseInnovationFromFile(string fileName);
	static string getStringBetweenQuotationMark(string line);

private:
	static bool contains(string line, string searched);
	static Genotype parseOneGenotype(ifstream &populationFile);
	static std::vector<NeuronGene> parseNeurons(ifstream &populationFile);
	static NeuronGene parseOneNeuron(ifstream &populationFile);
	static std::vector<LinkGene> parseLinks(ifstream &populationFile);
	static LinkGene parseOneLink(ifstream &populationFile);
	static InnovationElement parseOneInnovation(ifstream & innovationFile);
};

