# Genetic
Genetic is a C++ implementation of genetic algorithm. Doesn't include fitness function and doesn't do crossovers, only random mutations in form of bit flips. It could be used to play with various fitness functions or as a basis to add further features in terms of selection and breeding mechanisms.

## Usage example
This is from my attempt to solve killer sudoku with it.

    static const int bitset_size = 324;
    Genetic<CKillerSudoku, bitset_size>::genome_bitset candidate;
    Genetic<CKillerSudoku, bitset_size> genetic(candidate, &CKillerSudoku::fitnessFunction, this);
    genetic.SetGenomesInGeneration(1000);
    genetic.SetMutationRate(5);
    genetic.SetSurviveRate(800);
    genetic.SetVerbose(true);
    do
    {
        genetic.Generation(10);
    }
    while(genetic.GetBestScore() != 0);
    m_solution = convert(genetic.GetBestGenome());

## Licensing
Genetic is distributed under MIT licence as stated on http://opensource.org/licenses/MIT, which is very permissive. You can do anything you like with this code. Commercial use is allowed. See LICENCE file for details.

## Copyright
Copyright (c) 2012 Matej Zavrsnik <matejzavrsnik.com>