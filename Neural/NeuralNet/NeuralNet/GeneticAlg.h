#pragma once

#include <vector>

using namespace std;

typedef unsigned int uint;

struct Genome
{
	vector<float> m_vecWeights;

	float m_fitness;

	Genome() :m_fitness(0){}

	Genome(vector<float> w, float f) : m_vecWeights(w), m_fitness(f) {}

	friend bool operator < (const Genome& lhs, const Genome& rhs)
	{
		return (lhs.m_fitness < rhs.m_fitness);
	}

};

class GeneticAlg
{
private:
	// population of chromosomes
	vector<Genome> m_vecPop;

	// size of population
	uint m_populationSize;

	// weights per chromosome

	uint m_chomoSize;

	// total fitness of population
	float m_totalFitness;

	// best fitness in population
	float m_bestFitness;

	// average fitness
	float m_averageFitness;

	// worst fitness
	float m_worstFitness;

	// keeping track of best genome
	uint m_fittestGenome;

	// probability of mutation (0.05 to 0.3 or so)
	float m_mutationRate;

	// probability of crossover (0.7 is apparently alright)
	float m_crossoverRate;

	// generation counter
	uint m_generations;

	void Crossover(
		const vector<float> &mum,
		const vector<float> &dad,
		vector<float> &baby1,
		vector<float> &baby2);

	void Mutate(vector<float> &chromosome);

	Genome GetChromoRoulette();

	void GrabNBest(uint nBest, const uint numCopies, vector<Genome> &vecPop);

	void CalculateBestWorstAvgTot();

	void Reset();

public:
	GeneticAlg(uint a_popSize,
		float a_mutationRate,
		float a_crossoverRate,
		uint a_numWeights);

	// runs genetic algorithm for one iteration/generation
	vector<Genome> Epoch(vector<Genome> &old_pop);

	// accessors
	vector<Genome> GetChromosomes() const { return m_vecPop; }

	float AverageFitness() const { return m_totalFitness / m_populationSize; }

	float BestFitness() const { return m_bestFitness; }

};