#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <NEAT.h>
#include <memory>
#include "Genotype.h"
#include "Innovation.h"

namespace nev {

	class FileReader
	{
	public:
		static nev::NEAT getNEATFromFiles(const std::string& neatFile, const std::string& populationFile, const std::string& innovationFile);
		static std::vector<std::shared_ptr<nev::Genotype>> parsePopulationFromFile(const std::string& fileName);
		static Innovation parseInnovationFromFile(const std::string& fileName);
		static std::string getStringBetweenQuotationMark(const std::string& line);

	private:
		static bool contains(const std::string& line, const std::string& searched);
		static std::shared_ptr<Genotype> parseOneGenotype(std::ifstream& populationFile, int& genotypeID);
		static std::vector<NeuronGene> parseNeurons(std::ifstream& populationFile);
		static NeuronGene parseOneNeuron(std::ifstream& populationFile);
		static std::vector<LinkGene> parseLinks(std::ifstream& populationFile);
		static LinkGene parseOneLink(std::ifstream& populationFile);
		static InnovationElement parseOneInnovation(std::ifstream& innovationFile);
		static void parseNEATParameters(const std::string& fileName, NEAT& neat);
	};

}