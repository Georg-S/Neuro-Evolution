cmake_minimum_required (VERSION 3.8)
project(NeuroEvolution)

set(SOURCES 
	"src/FileReader.cpp"
	"src/FileWriter.cpp"
	"src/Genotype.cpp"
	"src/Innovation.cpp"
	"src/NEAT.cpp"
	"src/Phenotype.cpp"
	"src/RNG.cpp"
	"src/Species.cpp"
	"src/ActivationFunction.cpp"
	)

set(HEADERS 
	"include/FileReader.h" 
	"include/FileWriter.h" 
	"include/Genotype.h"
	"include/InnovationElement.h"
	"include/InnovationType.h"
	"include/Innovation.h"
	"include/LinkGene.h"
	"include/NEAT.h"
	"include/NeuronGene.h"
	"include/NeuronType.h"
	"include/ParentType.h"
	"include/Phenotype.h"
	"include/PhenotypeNeuron.h"
	"include/RNG.h"
	"include/Species.h"
	"include/ActivationFunction.h"
	)

add_library(NeuroEvolution STATIC ${SOURCES} ${HEADERS})
target_include_directories(NeuroEvolution PUBLIC include)