project(Unit_Tests)
add_subdirectory(lib)

set(TESTSOURCES 
	"TEST_Genotype.cpp"
	"TEST_FileManagment.cpp"
	"TEST_Innovation.cpp"
	"TEST_XOR.cpp"
	"TEST_Species.cpp"
	"TEST_Phenotype.cpp"
	"TEST_NEAT.cpp")


add_executable(Unit_Tests_run ${TESTSOURCES})
target_include_directories(Unit_Tests_run PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(Unit_Tests_run gtest gtest_main NeuroEvolution)