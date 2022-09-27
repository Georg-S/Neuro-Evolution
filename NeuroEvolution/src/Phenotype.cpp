#include "Phenotype.h"

nev::Phenotype::Phenotype(std::vector<std::unique_ptr<PhenotypeNeuron>>&& neurons, int maxDepth)
	: m_neurons(std::move(neurons))
	, m_maxDepth(maxDepth)
{
}

std::vector<double> nev::Phenotype::calculateOutputSnapshot(const std::vector<double>& inputs, const nev::af& activationFunction)
{
	setBias();
	setInputs(inputs);
	std::vector<double> output;
	auto func = nev::Activation::getFunction(activationFunction);

	for (int i = 0; i < m_maxDepth; i++)
		output = updateOnce(func);

	clearOutputOfNeurons();

	return output;
}

std::vector<double> nev::Phenotype::calculateOutputActive(const std::vector<double>& inputs, const nev::af& activationFunction)
{
	setBias();
	setInputs(inputs);
	auto func = nev::Activation::getFunction(activationFunction);

	return updateOnce(func);
}

void nev::Phenotype::setBias()
{
	for (auto& neuron : m_neurons)
	{
		if (neuron->neuronType == NeuronType::bias)
		{
			neuron->outputOfNeuron = 1;
			return;
		}
	}
}

void nev::Phenotype::setInputs(const std::vector<double>& inputs)
{
	int inputIndex = 0;
	for (auto& neuron : m_neurons)
	{
		if (inputIndex >= inputs.size())
			return;

		if (neuron->neuronType == NeuronType::input)
		{
			neuron->outputOfNeuron = inputs[inputIndex];
			inputIndex++;
		}
	}
}

std::vector<double> nev::Phenotype::updateOnce(const std::function<double(double input)>& activationFunction)
{
	std::vector<double> outputs;

	for (auto& neuron : m_neurons)
	{
		if (neuron->neuronType == NeuronType::input || neuron->neuronType == NeuronType::bias)
			continue;

		double sumOfActivation = 0.0;

		for (const auto& link : neuron->linksIn)
			sumOfActivation += (link.fromNeuron->outputOfNeuron * link.weight);

		neuron->outputOfNeuron = activationFunction(sumOfActivation);

		if (neuron->neuronType == NeuronType::output)
			outputs.emplace_back(neuron->outputOfNeuron);
	}
	return outputs;
}

void nev::Phenotype::clearOutputOfNeurons()
{
	for (auto& neuron : m_neurons)
		neuron->outputOfNeuron = 0.0;
}