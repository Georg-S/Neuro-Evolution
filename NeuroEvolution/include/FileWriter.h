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
	static void writePopulationToFile(string fileName, std::vector<Genotype> population);
	static void writeInnovationToFile(string fileName, Innovation &innovation);
private:
	static string stringifyGenotype(Genotype &genotype);
	static string stringifyInnovation(Innovation &innovation);
};

