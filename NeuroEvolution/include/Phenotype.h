#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "ActivationFunction.h"
#include "PhenotypeNeuron.h"

namespace nev
{
	class Phenotype
	{
	public:
		Phenotype(std::vector<std::unique_ptr<PhenotypeNeuron>>&& neurons, int maxDepth);
		std::vector <double> calculateOutputSnapshot(const std::vector <double>& input, const nev::af& activationFunction);
		std::vector <double> calculateOutputActive(const std::vector <double>& input, const nev::af& activationFunction);

	private:
		void setBias();
		void setInputs(const std::vector <double>& inputs);
		std::vector<double> updateOnce(const std::function<double(double input)>& activationFunction);
		void clearOutputOfNeurons();

		std::vector<std::unique_ptr<PhenotypeNeuron>> m_neurons;
		int m_maxDepth;
	};
}