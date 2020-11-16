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
	static void writePopulationToFile(const std::string &fileName, std::vector<Genotype> population);
	static void writeInnovationToFile(const std::string &fileName, const Innovation &innovation);
private:
	static std::string stringifyGenotype(const Genotype &genotype);
	static std::string stringifyInnovation(const Innovation &innovation);
};

