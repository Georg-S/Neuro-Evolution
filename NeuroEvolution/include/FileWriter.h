#pragma once

#include "Genotype.h"
#include "Innovation.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>


class FileWriter
{
public:
	FileWriter();
	~FileWriter();
	void writePopulationToFile(string fileName, std::vector<Genotype> population);
	void writeInnovationToFile(string fileName, Innovation &innovation);
private:
	string stringifyGenotype(Genotype &genotype);
	string stringifyInnovation(Innovation &innovation);
};

