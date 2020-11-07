#include "Phenotype.h"

Phenotype::Phenotype(vector<PhenotypeNeuron*> neurons, int maxDepth)
{
	this->neurons = neurons;
	this->maxDepth = maxDepth;
}


Phenotype::~Phenotype()
{
	for (int i = 0; i < neurons.size(); i++)
		delete neurons[i];
}

vector<double> Phenotype::calculateOutputSnapshot(const vector<double>& inputs)
{
	setBias();
	setInputs(inputs);
	vector<double> output;
	for (int i = 0; i < maxDepth; i++) {
		output = updateOnce();
	}
	clearOutputOfNeurons();

	return output;
}

vector<double> Phenotype::calculateOutputActive(const vector<double>& inputs)
{
	setBias();
	setInputs(inputs);

	return updateOnce();
}

void Phenotype::setBias()
{
	for (int i = 0; i < neurons.size(); i++) {
		if (neurons[i]->neuronType == bias) {
			neurons[i]->outputOfNeuron = 1;
			return;
		}
	}
}

void Phenotype::setInputs(const vector<double> &inputs)
{
	int inputIndex = 0;
	int inputsSize = inputs.size();
	for (int i = 0; i < neurons.size(); i++) {
		if (inputIndex >= inputsSize)
			return;

		if (neurons[i]->neuronType == input) {
			neurons[i]->outputOfNeuron = inputs[inputIndex];
			inputIndex++;
		}
	}
}

vector<double> Phenotype::updateOnce()
{
	vector<double> outputs;

	for (int neuronIndex = 0; neuronIndex < neurons.size(); neuronIndex++) {
		if (neurons[neuronIndex]->neuronType == input || neurons[neuronIndex]->neuronType == bias)
			continue;

		double sumOfActivation = 0.0;
		PhenotypeNeuron* currentNeuron = neurons[neuronIndex];

		for (int linkIndex = 0; linkIndex < currentNeuron->linksIn.size(); linkIndex++) {
			sumOfActivation += (currentNeuron->linksIn[linkIndex].fromNeuron->outputOfNeuron * currentNeuron->linksIn[linkIndex].weight);
		}
		currentNeuron->outputOfNeuron = activationFunction(sumOfActivation);

		if (currentNeuron->neuronType == output)
			outputs.push_back(currentNeuron->outputOfNeuron);
	}
	return outputs;
}

void Phenotype::clearOutputOfNeurons()
{
	for (int i = 0; i < neurons.size(); i++) {
		neurons[i]->outputOfNeuron = 0.0;
	}
}

double Phenotype::activationFunction(const double &input)
{
	return 1 / (1 + exp(-4.9*(input)));
}

