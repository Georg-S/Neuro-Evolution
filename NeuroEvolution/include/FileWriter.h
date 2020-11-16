#pragma once
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "Genotype.h"
#include "Innovation.h"


class FileWriter
{
public:
	static void writePopulationToFile(std::string fileName, std::vector<Genotype> population);
	static void writeInnovationToFile(std::string fileName, Innovation &innovation);
private:
	static std::string stringifyGenotype(Genotype &genotype);
	static std::string stringifyInnovation(const Innovation &innovation);
};

