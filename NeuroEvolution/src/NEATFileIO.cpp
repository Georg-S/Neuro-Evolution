#include "NeatFileIO.h"

using nlohmann::json;

static bool saveJson(const std::filesystem::path& path, const json& outputJson, const std::string& errorMessage = "Opening output file failed: ")
{
	try
	{
		auto file = std::ofstream(path);
		file << outputJson;
		file.close();
	}
	catch (const std::exception& e)
	{
		std::cout << errorMessage << e.what() << std::endl;
		return false;
	}

	return true;
}

static std::pair<bool, json> loadJson(const std::filesystem::path& path, const std::string& errorMessage = "Reading file failed: ")
{
	json parsedJson = {};

	try
	{
		std::ifstream ifs(path);
		parsedJson = json::parse(ifs);
		ifs.close();
	}
	catch (const std::exception& e)
	{
		std::cout << errorMessage << e.what() << std::endl;
		return { false, {} };
	}

	return { true, parsedJson };
}

bool nev::FileIO::writeHyperParametersToFile(const std::filesystem::path& path, const nev::NEAT& neat)
{
	json outputJson;

	//NEAT-Parameters
	outputJson["ActivationFunction"] = static_cast<int>(neat.activationFunction);
	outputJson["ExcessFactor"] = neat.excessFactor;
	outputJson["DisjointFactor"] = neat.disjointFactor;
	outputJson["WeightFactor"] = neat.weightFactor;
	outputJson["CompatibilityDistanceThreshold"] = neat.compatibilityDistanceThreshold;
	outputJson["GenerationsNoImprovementAllowed"] = neat.generationsNoImprovementAllowed;
	outputJson["SpeciesRoughValue"] = neat.speciesRoughValue;
	outputJson["MaxCountSpecies"] = neat.maxCountSpecies;
	outputJson["RecurrentAllowed"] = neat.recurrentAllowed;

	//Probabilities
	outputJson["CrossOverProbability"] = neat.crossOverProbability;
	outputJson["AddNeuronProbability"] = neat.addNeuronProbability;
	outputJson["AddLinkProbability"] = neat.addLinkProbability;
	outputJson["MutateLinkProbabilit"] = neat.mutateLinkProbability;
	outputJson["NewLinkWeightProbabiy"] = neat.newLinkWeightProbability;
	outputJson["WeightPertubation"] = neat.weightPertubation;

	outputJson["CurrentGeneration"] = neat.currentGeneration;

	return saveJson(path, outputJson);
}

bool nev::FileIO::writeInnovationsToFile(const std::filesystem::path& path, const Innovation& innovation)
{
	json outputJson;
	outputJson["CurrentInnovationNumber"] = innovation.getCurrentInnovationNumber();
	outputJson["StartNeuronNumber"] = innovation.getCurrentNeuronId();
	outputJson["Innovations"] = json::array();

	for (const auto& innovation : innovation.getInnovations())
	{
		auto innovationJson = json::object();

		innovationJson["FromID"] = innovation.fromNeuronId;
		innovationJson["InnovationID"] = innovation.innovationId;
		innovationJson["innovationType"] = static_cast<int>(innovation.innovationType);
		innovationJson["NeuronID"] = innovation.neuronId;
		innovationJson["ToID"] = innovation.toNeuronId;

		outputJson["Innovations"].emplace_back(std::move(innovationJson));
	}

	return saveJson(path, outputJson);
}

bool nev::FileIO::writePopulationToFile(const std::filesystem::path& path, const std::vector<std::shared_ptr<nev::Genotype>>& population)
{
	json outputJson;
	outputJson["Population"] = json::array();

	for (const auto& genotype : population)
	{
		auto genotypeJson = json::object();
		genotypeJson["CountOfInputs"] = genotype->getCountOfInputs();
		genotypeJson["CountOfOuputs"] = genotype->getCountOfOutputs();
		genotypeJson["MaxDepth"] = genotype->getMaxDepth();
		genotypeJson["Neurons"] = json::array();
		genotypeJson["Links"] = json::array();

		for (const auto& neuron : genotype->getNeurons())
		{
			auto neuronJson = json::object();

			neuronJson["Depth"] = neuron.depth;
			neuronJson["ID"] = neuron.id;
			neuronJson["NeuronType"] = static_cast<int>(neuron.neuronType);

			genotypeJson["Neurons"].emplace_back(std::move(neuronJson));
		}

		for (const auto& link : genotype->getLinks())
		{
			auto linkJson = json::object();

			linkJson["Enabled"] = link.enabled;
			linkJson["FromID"] = link.fromNeuronID;
			linkJson["InnovationID"] = link.innovationID;
			linkJson["Recurrent"] = link.recurrent;
			linkJson["ToID"] = link.toNeuronID;
			linkJson["Weight"] = link.weight;

			genotypeJson["Links"].emplace_back(std::move(linkJson));
		}

		outputJson["Population"].emplace_back(std::move(genotypeJson));
	}

	return saveJson(path, outputJson);
}

std::vector<std::shared_ptr<nev::Genotype>> nev::FileIO::loadPopulationFromFile(const std::filesystem::path& file, bool* outSuccess)
{
	auto [success, populationJson] = loadJson(file);
	*outSuccess &= success;
	auto population = std::vector<std::shared_ptr<nev::Genotype>>();
	int id = 0;

	if (!success)
		return {};

	for (const auto& genotypeJson : populationJson["Population"])
	{
		int countOfInputs = genotypeJson["CountOfInputs"];
		int countOfOutputs = genotypeJson["CountOfOuputs"];
		int maxDepth = genotypeJson["MaxDepth"];
		std::vector<nev::NeuronGene> neurons;
		std::vector<nev::LinkGene> links;

		for (const auto& neuronJson : genotypeJson["Neurons"])
		{
			NeuronGene neuron = {};
			neuron.depth = neuronJson["Depth"];
			neuron.id = neuronJson["ID"];
			neuron.neuronType = static_cast<NeuronType>(neuronJson["NeuronType"]);
			neurons.emplace_back(std::move(neuron));
		}

		for (const auto& linkJson : genotypeJson["Links"])
		{
			LinkGene link = {};
			link.enabled = linkJson["Enabled"];
			link.fromNeuronID = linkJson["FromID"];
			link.innovationID = linkJson["InnovationID"];
			link.recurrent = linkJson["Recurrent"];
			link.toNeuronID = linkJson["ToID"];
			link.weight = linkJson["Weight"];
			links.emplace_back(std::move(link));
		}

		population.emplace_back(std::make_shared<nev::Genotype>(neurons, links, id++));
	}

	return population;
}

nev::Innovation nev::FileIO::loadInnovationFromFile(const std::filesystem::path& file, bool* outSuccess)
{
	auto [success, innovationJson] = loadJson(file);
	*outSuccess &= success;

	if (!success)
		return {};

	std::vector<InnovationElement> innovations;
	int currentInnovationNumber = innovationJson["CurrentInnovationNumber"];
	int startNeuronNumber = innovationJson["StartNeuronNumber"];
	for (const auto& innovationElementJson : innovationJson["Innovations"])
	{
		int fromId = innovationElementJson["FromID"];
		int innovationId = innovationElementJson["InnovationID"];
		InnovationType innovationType = static_cast<InnovationType>(innovationElementJson["innovationType"]);
		int neuronId = innovationElementJson["NeuronID"];
		int toId = innovationElementJson["ToID"];

		innovations.emplace_back(innovationId, fromId, toId, neuronId, innovationType);
	}

	return Innovation(currentInnovationNumber, startNeuronNumber, innovations);
}

void nev::FileIO::loadNEATParametersFromFile(const std::filesystem::path& file, NEAT* outNeat, bool* outSuccess)
{
	auto [success, parameterJson] = loadJson(file);
	*outSuccess &= success;

	if (!success)
		return;

	//NEAT-Parameters
	outNeat->activationFunction = static_cast<nev::af>(parameterJson["ActivationFunction"]);
	outNeat->excessFactor = parameterJson["ExcessFactor"];
	outNeat->disjointFactor = parameterJson["DisjointFactor"];
	outNeat->weightFactor = parameterJson["WeightFactor"];
	outNeat->compatibilityDistanceThreshold = parameterJson["CompatibilityDistanceThreshold"];
	outNeat->generationsNoImprovementAllowed = parameterJson["GenerationsNoImprovementAllowed"];
	outNeat->speciesRoughValue = parameterJson["SpeciesRoughValue"];
	outNeat->maxCountSpecies = parameterJson["MaxCountSpecies"];
	outNeat->recurrentAllowed = parameterJson["RecurrentAllowed"];

	//Probabilities
	outNeat->crossOverProbability = parameterJson["CrossOverProbability"];
	outNeat->addNeuronProbability = parameterJson["AddNeuronProbability"];
	outNeat->addLinkProbability = parameterJson["AddLinkProbability"];
	outNeat->mutateLinkProbability = parameterJson["MutateLinkProbabilit"];
	outNeat->newLinkWeightProbability = parameterJson["NewLinkWeightProbabiy"];
	outNeat->weightPertubation = parameterJson["WeightPertubation"];

	outNeat->currentGeneration = parameterJson["CurrentGeneration"];
	outNeat->refreshPopulationActivationFunction();
}

bool nev::FileIO::saveToFile(const std::filesystem::path& basePath, const nev::NEAT& neat)
{
	auto parametersPath = basePath;
	auto innovationsPath = basePath;
	auto populationPath = basePath;
	parametersPath.append("neat_parameters.json");
	innovationsPath.append("neat_innovations.json");
	populationPath.append("neat_population.json");

	bool success = writeHyperParametersToFile(parametersPath, neat)
		&& writeInnovationsToFile(innovationsPath, neat.innovation)
		&& writePopulationToFile(populationPath, neat.population);

	return success;
}

std::unique_ptr<nev::NEAT> nev::FileIO::loadNEATFromFiles(const std::filesystem::path& parametersPath,
	const std::filesystem::path& populationPath, const std::filesystem::path& innovationPath)
{
	bool success = true;
	try
	{
		auto population = loadPopulationFromFile(populationPath, &success);
		auto innovation = loadInnovationFromFile(innovationPath, &success);
		auto neat = std::make_unique<nev::NEAT>(population, innovation);
		neat->currentGenotypeId = population.size();
		loadNEATParametersFromFile(parametersPath, neat.get(), &success);

		if (!success)
		{
			std::cout << "Error occured while opening files" << std::endl;
			return nullptr;
		}

		return neat;
	}
	catch (const std::exception& e)
	{
		std::cout << "Error occured while loading files, json is malformed: " << e.what() << std::endl;
	}

	return nullptr;
}
