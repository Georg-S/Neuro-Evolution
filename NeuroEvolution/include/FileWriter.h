#pragma once
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "NEAT.h"
#include "Genotype.h"
#include "Innovation.h"

namespace nev {

	class FileWriter
	{
	public:
		static void writeNEATToFile(const nev::NEAT& neat, const std::string& fileSuffix = "");
		static void writeNEATParametersToFile(const std::string& fileName, const nev::NEAT& neat);
		static void writePopulationToFile(const std::string& fileName, std::vector<Genotype> population);
		static void writeInnovationToFile(const std::string& fileName, const Innovation& innovation);
	private:
		static std::string stringifyGenotype(const Genotype& genotype);
		static std::string stringifyInnovation(const Innovation& innovation);
		static std::string stringifyNEAT(const NEAT& neat);

		template <typename T>
		static std::string to_string_with_max_precision(const T& a_value);
	};

}