#pragma once
#include <vector>
#include <functional>
#include "ActivationFunction.h"
#include "PhenotypeNeuron.h"

namespace nev {

	class Phenotype
	{
	public:
		Phenotype(const std::vector<PhenotypeNeuron*>& neurons, int maxDepth);
		~Phenotype();
		std::vector <double> calculateOutputSnapshot(const std::vector <double>& input,
			const nev::af& activationFunction);
		std::vector <double> calculateOutputActive(const std::vector <double>& input,
			const nev::af& activationFunction);
	private:
		void setBias();
		void setInputs(const std::vector <double>& inputs);
		std::vector<double> updateOnce(const std::function<double(double input)>& activationFunction);
		void clearOutputOfNeurons();

		std::vector<PhenotypeNeuron*> neurons;
		int maxDepth;
	};

}