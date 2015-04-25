#include "NeuralNet.h"
#include "utility.h"

Neuron::Neuron(uint numInputs) : m_inputs(numInputs + 1)
{
	for (uint i = 0; i < numInputs + 1; ++i)
	{
		//randomly weight the inputs
		m_vecWeight.push_back(RandomClamped());
	}
}

NeuronLayer::NeuronLayer(uint a_neuronNumber, uint a_inputsPerNeuron)
{
	m_numNeurons = a_neuronNumber;
	for (int i = 0; i < a_neuronNumber; ++i)
	{
		m_vecNeurons.push_back(Neuron(a_inputsPerNeuron));
	}
}

NeuralNet::NeuralNet(uint numInputs, uint numOutputs, uint numHiddenLayers, uint numNeuronsPerHidden)
{
	m_NumInputs = numInputs;
	m_NumOutputs = numOutputs;
	m_NumHiddenLayers = numHiddenLayers;
	m_NeuronsPerHiddenLayer = numNeuronsPerHidden;

	CreateNet();
}

void NeuralNet::CreateNet()
{
	// create layers
	if (m_NumHiddenLayers > 0)
	{
		// create first layer
		m_vecLayers.push_back(NeuronLayer(m_NeuronsPerHiddenLayer, m_NumInputs));

		for (int i = 0; i < m_NumHiddenLayers - 1; ++i)
		{
			m_vecLayers.push_back(NeuronLayer(m_NeuronsPerHiddenLayer, m_NeuronsPerHiddenLayer));
		}

		// create output layer
		m_vecLayers.push_back(NeuronLayer(m_NumOutputs, m_NeuronsPerHiddenLayer));
	}
	else
	{
		// create output layer
		m_vecLayers.push_back(NeuronLayer(m_NumOutputs, m_NumInputs));
	}
}

vector<float> NeuralNet::GetWeights() const
{
	vector<float> weights;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers + 1; i++)
	{
		// for each neuron
		for (int j = 0; j < m_vecLayers[i].m_numNeurons; j++)
		{
			// for each weight
			for (int k = 0; k < m_vecLayers[i].m_vecNeurons[j].m_inputs; k++)
			{
				weights.push_back(m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k]);
			}
		}
	}

	return weights;
}

void NeuralNet::PutWeights(vector<float> &weights)
{
	int cWeight = 0;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers + 1; i++)
	{
		// for each neuron
		for (int j = 0; j < m_vecLayers[i].m_numNeurons; j++)
		{
			// for each weight
			for (int k = 0; k < m_vecLayers[i].m_vecNeurons[j].m_inputs; k++)
			{
				m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k] = weights[cWeight++];
			}
		}
	}

	return;
}

uint NeuralNet::GetNumberOfWeights() const
{
	int weights = 0;

	// for each layer
	for (int i = 0; i < m_NumHiddenLayers+1; i++)
	{
		// for each neuron
		for (int j = 0; j < m_vecLayers[i].m_numNeurons; j++)
		{
			// for each weight
			for (int k = 0; k < m_vecLayers[i].m_vecNeurons[j].m_inputs; k++)
			{
				weights++;
			}
		}
	}
	return weights;
}

vector<float> NeuralNet::Update(vector<float> &inputs)
{
	vector<float> outputs;

	int nWeight = 0;

	// check we have correct # of inputs
	if (inputs.size() != m_NumInputs)
	{
		return outputs;
	}

	for (int i = 0; i < m_NumHiddenLayers + 1; ++i)
	{
		if (i > 0)
		{
			inputs = outputs;
		}

		outputs.clear();

		nWeight = 0;

		// for each neuron sum the inputs * corresponding weights
		for (int j = 0; j < m_vecLayers[i].m_numNeurons; ++j)
		{
			float netinput = 0;
			int numInputs = m_vecLayers[i].m_vecNeurons[j].m_inputs;

			//for each weight
			for (int k = 0; k < numInputs - 1; ++k)
			{
				// sum weights * inputs
				netinput += m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k] * inputs[nWeight++];
			}

			// add in bias
			netinput += m_vecLayers[i].m_vecNeurons[j].m_vecWeight[numInputs - 1] * NODEBIAS; // bias (negative 1)

			// store outputs from each layer as they're generated
			outputs.push_back(Sigmoid(netinput, ACTIVATIONRESPONSE)); // activation response (currently 1)

			nWeight = 0;

		}

	}

	return outputs;
}

float NeuralNet::Sigmoid(float netInput, float response)
{
	return (1 / (1 + exp(-netInput / response)));
}