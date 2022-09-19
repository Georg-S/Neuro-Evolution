#pragma once
#include <string>
#include <algorithm>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "json.hpp"
#include "NEAT.h"
#include "Genotype.h"
#include "Innovation.h"

namespace nev
{
	class FileIO
	{
	public:
		static bool saveToFile(const std::filesystem::path& basePath, const nev::NEAT& neat);
		static std::unique_ptr<nev::NEAT> loadNEATFromFiles(const std::filesystem::path& parametersPath,
			const std::filesystem::path& populationPath, const std::filesystem::path& innovationPath);

	private:
		static bool writeHyperParametersToFile(const std::filesystem::path& path, const nev::NEAT& neat);
		static bool writeInnovationsToFile(const std::filesystem::path& path, const Innovation& innovation);
		static bool writePopulationToFile(const std::filesystem::path& path, const std::vector<std::shared_ptr<nev::Genotype>>& population);
		static std::vector<std::shared_ptr<nev::Genotype>> loadPopulationFromFile(const std::filesystem::path& file, bool* outSuccess);
		static Innovation loadInnovationFromFile(const std::filesystem::path& file, bool* outSuccess);
		static void loadNEATParametersFromFile(const std::filesystem::path& file, NEAT* outNeat, bool* outSuccess);
	};
}