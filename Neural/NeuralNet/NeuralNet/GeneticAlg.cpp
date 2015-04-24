#include "GeneticAlg.h"
#include "utility.h"
#include <algorithm>

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
	// traverse the chromosome and mutate each weight
	// dependent on mutation rate
	for (uint i = 0; i < chromosomes.size(); i++)
	{
		// perturb or not?
		if (RandFloat() < m_mutationRate)
		{
			// perturb
			chromosomes[i] += (RandomClamped() * MAXPERTURBATION); // 0.1f is max perturbation
		}
	}
}

// roulette wheel sampling of chromosomes
Genome GeneticAlg::GetChromoRoulette()
{
	// generate a random number between 0 and total fitness count
	float slice = RandFloat() * m_totalFitness;

	Genome ChosenOne;

	// cumulative total of fitnesses
	float FitnessSoFar = 0;

	for (uint i = 0; i < m_populationSize; ++i)
	{
		FitnessSoFar += m_vecPop[i].m_fitness;

		// if the fitness so far > random number
		// then return chromo at that point
		if (FitnessSoFar >= slice)
		{
			ChosenOne = m_vecPop[i];
			break;
		}
	}
	return ChosenOne;
}

void GeneticAlg::Crossover(const vector<float> &mum,
	const vector<float> &dad,
	vector<float> &baby1,
	vector<float> &baby2)
{
	// return parents as offspring if parents are same or if
	// crossover doesn't occur
	if ( ( RandFloat() > m_crossoverRate ) || (mum == dad) ) 
	{
		baby1 = mum;
		baby2 = dad;
	
		return;
	}

	// determine a crossover point
	int crossover = RandInt(0, m_chomoSize - 1);

	// create offspring
	for (int i = 0; i < crossover; i++)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i = crossover; i < mum.size(); i++)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}

	return;
}

vector<Genome> GeneticAlg::Epoch(vector<Genome> &old_pop)
{
	// assign given population
	m_vecPop = old_pop;

	// reset appropriate variables
	Reset();

	// sort population (for scaling and elitism)
	sort(m_vecPop.begin(), m_vecPop.end());

	// calculate best, worst, average, total fitnesses
	CalculateBestWorstAvgTot();

	// create temporary vector for outputs
	vector<Genome> OutputPop;

	// add elitism. copy fittest genomes. add even number or roulette crashes
	if (!(NUMCOPIESELITE * NUMELITE % 2))
	{
		GrabNBest(NUMELITE, NUMCOPIESELITE, OutputPop);
	}

	// enter into the genetic algorithm looperino
	
	// repeat until new population is generated
	while (OutputPop.size() < m_populationSize)
	{
		// choose parent chromosomes
		Genome mum = GetChromoRoulette();
		Genome dad = GetChromoRoulette();

		// create offspring via crossover
		vector<float> baby1, baby2;

		Crossover(mum.m_vecWeights, dad.m_vecWeights, baby1, baby2);

		// mutate
		Mutate(baby1);
		Mutate(baby2);

		// copy into new population
		OutputPop.push_back(Genome(baby1, 0));
		OutputPop.push_back(Genome(baby2, 0));
	}

	// reassign population

	m_vecPop = OutputPop;

	return m_vecPop;
}

// inserting numCopies copies of nBest fittest genomes into population
void GeneticAlg::GrabNBest(uint nBest,
	const uint numCopies,
	vector<Genome> &Population)
{
	// add required number of copies of the n most fittest to vector
	while (nBest--)
	{
		for (uint i = 0; i < numCopies; i++)
		{
			Population.push_back(m_vecPop[(m_populationSize - 1) - nBest]);
		}
	}
}

// calculates fittest, weakest genomes and average/total fitnesses
void GeneticAlg::CalculateBestWorstAvgTot()
{
	m_totalFitness = 0;

	float highestSoFar = 0;
	float lowestSoFar = 9999999;

	for (int i = 0; i < m_populationSize; i++)
	{
		// update fittest if necessary
		if (m_vecPop[i].m_fitness > highestSoFar)
		{
			highestSoFar = m_vecPop[i].m_fitness;
			m_fittestGenome = i;
			m_bestFitness = highestSoFar;
		}

		// update worst if necessary
		if (m_vecPop[i].m_fitness < lowestSoFar)
		{
			lowestSoFar = m_vecPop[i].m_fitness;
			m_worstFitness = lowestSoFar;
		}

		m_totalFitness += m_vecPop[i].m_fitness;
	}

	m_averageFitness = m_totalFitness / m_populationSize;
}

// resets relevant variables for new generation
void GeneticAlg::Reset()
{
	m_totalFitness = 0;
	m_bestFitness = 0;
	m_worstFitness = 9999999;
	m_averageFitness = 0;
}