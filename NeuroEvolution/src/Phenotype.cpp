#include "Phenotype.h"

Phenotype::Phenotype(const std::vector<PhenotypeNeuron*>& neurons, const int& maxDepth)
{
	this->neurons = neurons;
	this->maxDepth = maxDepth;
}


Phenotype::~Phenotype()
{
	for (int i = 0; i < neurons.size(); i++)
		delete neurons[i];
}

std::vector<double> Phenotype::calculateOutputSnapshot(const std::vector<double>& inputs,
	const std::function<double(const double& input)>& activationFunction)
{
	setBias();
	setInputs(inputs);
	std::vector<double> output;
	for (int i = 0; i < maxDepth; i++) {
		output = updateOnce(activationFunction);
	}
	clearOutputOfNeurons();

	return output;
}

std::vector<double> Phenotype::calculateOutputActive(const std::vector<double>& inputs,
	const std::function<double(const double& input)>& activationFunction)
{
	setBias();
	setInputs(inputs);

	return updateOnce(activationFunction);
}

void Phenotype::setBias()
{
	for(PhenotypeNeuron* neuron : neurons) {
		if (neuron->neuronType == NeuronType::bias) {
			neuron->outputOfNeuron = 1;
			return;
		}
	}
}

void Phenotype::setInputs(const std::vector<double> &inputs)
{
	int inputIndex = 0;
	for(PhenotypeNeuron* neuron : neurons) {
		if (inputIndex >= inputs.size())
			return;

		if (neuron->neuronType == NeuronType::input) {
			neuron->outputOfNeuron = inputs[inputIndex];
			inputIndex++;
		}
	}
}

std::vector<double> Phenotype::updateOnce(const std::function<double(const double& input)>& activationFunction)
{
	std::vector<double> outputs;

	for(PhenotypeNeuron* neuron : neurons) {
		if (neuron->neuronType == NeuronType::input || neuron->neuronType == NeuronType::bias)
			continue;

		double sumOfActivation = 0.0;

		for(PhenotypeNeuron::Link link : neuron->linksIn)
			sumOfActivation += (link.fromNeuron->outputOfNeuron * link.weight);
		
		neuron->outputOfNeuron = activationFunction(sumOfActivation);

		if (neuron->neuronType == NeuronType::output)
			outputs.push_back(neuron->outputOfNeuron);
	}
	return outputs;
}

void Phenotype::clearOutputOfNeurons()
{
	for (int i = 0; i < neurons.size(); i++)
		neurons[i]->outputOfNeuron = 0.0;
}