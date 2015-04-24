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
private:
	uint m_NumInputs;
	uint m_NumOutputs;
	uint m_NumHiddenLayers;
	uint m_NeuronsPerHiddenLayer;

	// storage for each layer of neurons (including output)
	vector<NeuronLayer> m_vecLayers;

public:
	NeuralNet(uint numInputs, uint numOutputs, uint numHiddenLayers, uint numNeuronsPerHidden);

	void CreateNet();

	// retrieve weights
	vector<float> GetWeights() const;

	// returns total number of weights in net
	uint GetNumberOfWeights() const;

	// replaces weights
	void PutWeights(vector<float> &weights);

	// calculates outputs based on a set of inputs
	vector<float> Update(vector<float> &inputs);

	inline float Sigmoid(float activation, float response);
};