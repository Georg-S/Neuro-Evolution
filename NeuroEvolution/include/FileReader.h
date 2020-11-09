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
	static std::vector<Genotype> parsePopulationFromFile(std::string fileName);
	static Innovation parseInnovationFromFile(std::string fileName);
	static std::string getStringBetweenQuotationMark(std::string line);

private:
	static bool contains(std::string line, std::string searched);
	static Genotype parseOneGenotype(std::ifstream &populationFile);
	static std::vector<NeuronGene> parseNeurons(std::ifstream &populationFile);
	static NeuronGene parseOneNeuron(std::ifstream &populationFile);
	static std::vector<LinkGene> parseLinks(std::ifstream &populationFile);
	static LinkGene parseOneLink(std::ifstream &populationFile);
	static InnovationElement parseOneInnovation(std::ifstream & innovationFile);
};

