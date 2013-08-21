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

#ifndef GENETIC_H
#define	GENETIC_H

#include <bitset>
#include <vector>
#include <functional>

#define GENOME_TYPE _genomeType
#define CLASS_TYPE _Type
#define TEMPLATE template<class CLASS_TYPE, size_t GENOME_SIZE>
#define GENETIC Genetic<CLASS_TYPE, GENOME_SIZE>

TEMPLATE class Genetic
{
public:
    using genome_bitset = std::bitset<GENOME_SIZE>;
    using fitness_function = std::function<int(CLASS_TYPE*, const Genetic::genome_bitset&)>;
    Genetic(genome_bitset genomeSeed, fitness_function fitnessFunction, CLASS_TYPE* object);
    ~Genetic();

    void Generation( int count );

    genome_bitset GetBestGenome() const;
    int GetBestScore() const;
    int GetGenerationCount() const;
    void SetVerbose(const bool verbose);
    void SetSurviveRate(const uint surviveRate);
    uint GetSurviveRate() const;
    void SetMutationRate(const uint mutationRate);
    uint GetMutationRate() const;
    void SetGenomesInGeneration(const uint genomesInGeneration);
    uint GetGenomesInGeneration() const;

private:
    struct TGenome
    {
        genome_bitset genome;
        int score;
    };

    int GetRandom() const;
    void MutateCurrentGeneration();
    void MutateGenome(TGenome& genome);
    void MultiplyBestGenomes();

    std::vector<TGenome> m_genomePool;
    TGenome m_bestSoFar;
    fitness_function m_fitnessFunction;
    CLASS_TYPE* m_fitnessFunctionObject;
    int m_generations;
    const int m_randomCeiling; // to avoid modulo bias

    // settings
    uint m_genomesInGeneration; // genome pool size
    uint m_mutationRate; // how many bits to mutate
    uint m_surviveRate; // how many genomes should survive generation and multiply
    bool m_verbose; // debug
};

#endif	/* GENETIC_H */

