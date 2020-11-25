#pragma once

#include "Genotype.h"
#include "Innovation.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

class FileWriter
{
public:
	FileWriter();
	~FileWriter();
	static void writePopulationToFile(string fileName, vector<Genotype> population);
	static void writeInnovationToFile(string fileName, Innovation &innovation);
private:
	static string stringifyGenotype(Genotype &genotype);
	static string stringifyInnovation(Innovation &innovation);
};

