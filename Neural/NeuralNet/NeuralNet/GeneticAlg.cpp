#include "GeneticAlg.h"
#include "utility.h"

GeneticAlg::GeneticAlg(uint a_popSize,
	float a_mutationRate,
	float a_crossoverRate,
	uint a_numWeights) : 
	m_populationSize(a_popSize),
	m_mutationRate(a_mutationRate),
	m_crossoverRate(a_crossoverRate),
	m_chomoSize(a_numWeights),
	m_totalFitness(0),
	m_generations(0),
	m_fittestGenome(0),
	m_bestFitness(0),
	m_worstFitness(999999999),
	m_averageFitness(0)
{
	// initialise population with random chromosomes
	// fitnesses all set to zero
	for (uint i = 0; i < m_populationSize; ++i)
	{
		m_vecPop.push_back(Genome());
		for (uint j = 0; j < m_chomoSize; ++j)
		{
			m_vecPop[i].m_vecWeights.push_back(RandomClamped());
		}
	}
}

void GeneticAlg::Mutate(vector<float> &chromosomes)
{

}