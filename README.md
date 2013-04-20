# Genetic
Genetic is a C++ implementation of genetic algorithm. Doesn't include fitness function and doesn't do crossovers, only random mutations in form of bit flips. Selection operator only chooses best solution, no other selection mechanisms are implemented. It could be used to play with various fitness functions or as a basis to add further features in terms of selection and breeding mechanisms.

## Usage example
This is how you could solve knapsack problem for instance.

    // Include
    #include "genetic.h"
    
    // Define and initialize problem
    struct TItem
    {
      int weight;
      int value;
    } knapsackItems[16] = { {15,4}, {13,7}, {3,4}, {5,22}, {77,3}, ... }; // 16 items
    #define GENESIZE 2
    char seedGene[GENESIZE] = {0,0}; // start with empty solution, "seed gene"
    #define MAX_WEIGHT 70 // maximum size of knapsack
    
    // create fitness function for the problem
    int evalGene(char* gene) {
      int score = 0;
      int sumweight = 0;
      int index;
      for(int byte=0; byte<GENESIZE; byte++) {
        for(int bit=0; bit<8; bit++) {
          if(gene[byte] & (1 << bit)) { // if bit toggled, account for it's weight
            index = (byte*8)+bit;
            score += knapsackItems[index].value;
            sumweight += knapsackItems[index].weight;
            if(sumweight > MAX_WEIGHT) {
              return 0;
            }
          }
        }
      }
      return score;
    }
    
    // Feed requirements to the algorithm and instantiate it
    Genetic gen(seedGene, GENESIZE, &evalGene);
    // Then either loop, until the solution satisfies some requirements
    int curScore = 0;
    do {
      gen.Generation(1);
      if(curScore < gen.GetBestScore()) {
        curScore = gen.GetBestScore();
      }
      // do something with current best?
      char* currentBest = gen.GetBestGene();
    }
    while( /* requirements met? */ );
    // Or simply allow 10.000 generations to unfold, then use that solution
    gen.Generation(10000);
    char* solution = gen.GetBestGene();

## Licensing
Genetic is distributed under MIT licence as stated on http://opensource.org/licenses/MIT, which is very permissive. You can do anything you like with this code. Commercial use is allowed. See LICENCE file for details.

## Copyright
Copyright (c) 2012 Matej Zavrsnik <matejzavrsnik.com>