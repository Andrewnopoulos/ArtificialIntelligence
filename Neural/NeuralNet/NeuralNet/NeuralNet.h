#pragma once

#include <vector>

typedef unsigned int uint;

using namespace std;

struct Neuron
{
	// number of inputs
	uint m_inputs;

	// weight for each input
	vector<float> m_vecWeight;

	// constructor
	Neuron(uint numInputs);

};

struct NeuronLayer
{
	// number of neurons in layer
	uint m_numNeurons;

	// layer of neurons
	vector<Neuron> m_vecNeurons;

	NeuronLayer(uint a_neuronNumber, uint a_inputsPerNeuron);

};

class NeuralNet
{

};