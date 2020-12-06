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
	"src/HNE/HistoricalGenotype.cpp"
	"src/HNE/HNE.cpp"
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
	"include/HNE/HistoricalGenotype.h"
	"include/HNE/HNE.h"
	)

add_library(NeuroEvolution STATIC ${SOURCES} ${HEADERS})
target_include_directories(NeuroEvolution PUBLIC "include")


set(NEAT_LIB_PATH ${CMAKE_CURRENT_BINARY_DIR}"/"$<CONFIG>"/NeuroEvolution.lib")

add_custom_command(
        TARGET NeuroEvolution POST_BUILD
        COMMAND if $<CONFIG:Release> neq 0 ( ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}"/NeuroEvolution/include"
                ${CMAKE_SOURCE_DIR}"/Unreal_Engine_Project/Source/UnrealBachelor/libs/NeuroEvolution/include")
				)

add_custom_command(
		TARGET NeuroEvolution POST_BUILD
		COMMAND if $<CONFIG:Release> neq 0 ( ${CMAKE_COMMAND} -E copy
				${NEAT_LIB_PATH}
				${CMAKE_SOURCE_DIR}"/Unreal_Engine_Project/Source/UnrealBachelor/libs/NeuroEvolution/bin/NeuroEvolution.lib")
			)