/*
Copyright (C) 2012 Matej Zavrsnik <matejzavrsnik@gmail.com> (matejzavrsnik.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "genetic.h"
#include <vector>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <climits>
#include <iostream>
#include "tools.h"

TEMPLATE GENETIC::Genetic(genome_bitset genomeSeed, fitness_function fitnessFunction, CLASS_TYPE* object) :
    m_randomCeiling(RAND_MAX - (RAND_MAX % (2-1)))
{
    m_fitnessFunction = fitnessFunction;
    m_fitnessFunctionObject = object;
    // set some meaningful settings
    m_genomesInGeneration = 1000; // how many different genes in generation
    m_mutationRate = 5; // how many bits to zap each generation?
    m_surviveRate = 800;
    m_verbose = false;
    // create seed gene
    m_bestSoFar.genome = genomeSeed;
    m_bestSoFar.score = m_fitnessFunction(m_fitnessFunctionObject, genomeSeed);
    // create initial population from seed gene
    for(int i=0; i<m_genomesInGeneration; ++i)
    {
        m_genomePool.push_back(m_bestSoFar);
    }
    m_generations = 0;
    // initialize randomizer
    srand(1247+time(NULL));
};

TEMPLATE GENETIC::~Genetic()
{
}

TEMPLATE void GENETIC::Generation( int count )
{
    for(int i=0; i<count; ++i)
    {
        MutateCurrentGeneration();
        MultiplyBestGenomes();
        ++m_generations;
    }
}

TEMPLATE typename GENETIC::genome_bitset GENETIC::GetBestGenome() const
{
    return m_genomePool[0].genome;
}

TEMPLATE int GENETIC::GetBestScore() const
{
    return m_genomePool[0].score;
}

TEMPLATE int GENETIC::GetGenerationCount() const
{
    return m_generations;
}

TEMPLATE void GENETIC::SetVerbose(const bool verbose) 
{
    m_verbose = verbose;
}

TEMPLATE void GENETIC::SetSurviveRate(const uint surviveRate) 
{
    m_surviveRate = surviveRate;
}

TEMPLATE uint GENETIC::GetSurviveRate() const 
{
    return m_surviveRate;
}

TEMPLATE void GENETIC::SetMutationRate(const uint mutationRate) 
{
    m_mutationRate = mutationRate;
}

TEMPLATE uint GENETIC::GetMutationRate() const 
{
    return m_mutationRate;
}

TEMPLATE void GENETIC::SetGenomesInGeneration(uint genomesInGeneration) 
{
    m_genomesInGeneration = genomesInGeneration;
}

TEMPLATE uint GENETIC::GetGenomesInGeneration() const 
{
    return m_genomesInGeneration;
}

TEMPLATE int GENETIC::GetRandom() const
{
    // without modulo bias
    int number;
    do
    {
        number = rand();
    }
    while(number > m_randomCeiling);
    number = number % (GENOME_SIZE-1);
    return number;
}

TEMPLATE void GENETIC::MutateCurrentGeneration()
{
    // mutate whole population skipping the best one, so start from 1
    for( int i=1; i<m_genomesInGeneration; ++i)
    {
        MutateGenome(m_genomePool[i]);
    }
}

TEMPLATE void GENETIC::MutateGenome(TGenome& genome)
{
    for(int rate=0; rate<m_mutationRate; ++rate)
    {
        // flip random bit
        size_t whichBit = GetRandom();
        genome.genome[whichBit].flip();
        genome.score = m_fitnessFunction(m_fitnessFunctionObject, genome.genome);
    }
}

TEMPLATE void GENETIC::MultiplyBestGenomes()
{
    std::sort(m_genomePool.begin(), m_genomePool.end(), 
        [](const TGenome& a, const TGenome& b) 
        { 
            return a.score > b.score; 
        });
    for(int i=m_surviveRate, j=0; i<m_genomePool.size(); ++i, ++j)
    {
        if(j>=m_surviveRate) j=0;
        m_genomePool[i] = m_genomePool[j];
    }
}
