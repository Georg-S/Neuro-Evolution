project(Unit_Tests)
add_subdirectory(lib)
enable_testing()

set_target_properties(gmock gmock_main gtest gtest_main PROPERTIES FOLDER GoogleTestTargets)

set(TESTSOURCES 
	"TEST_Genotype.cpp"
	"TEST_FileManagment.cpp"
	"TEST_Innovation.cpp"
	"TEST_XOR.cpp"
	"TEST_Species.cpp"
	"TEST_Phenotype.cpp"
	"TEST_NEAT.cpp"
	"TEST_HistoricalGenotype.cpp"
	"TEST_HNE.cpp"
	"Test_config.h"
	)


add_executable(Unit_Tests ${TESTSOURCES})
target_link_libraries(Unit_Tests  gtest gtest_main NeuroEvolution)
add_test(NAME Unit_Tests COMMAND Unit_Tests)