#include "NeuralNet.h"

Neuron::Neuron(uint numInputs) : m_inputs(numInputs + 1)
{
	for (uint i = 0; i < numInputs + 1; ++i)
	{
		//randomly weight the inputs
		m_vecWeight.push_back(rand() % 10);
	}
}