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

struct TGene
{
  char* gene;
  int score;
};

class Genetic
{
public:
  Genetic( char* geneSeed, int geneSize, int (*evalGene) (char*) );
  ~Genetic();

  void Generation( int count );

  char* GetBestGene();
  int GetBestScore();
  int GetGenerationCount();

private:
  void CopyGene( char* src, char* dest );
  void CopyGene( TGene& src, TGene& dest );
  void CopyBestToRest();
  int GetRandom( int max );
  void Mutate();
  void Sort();

  int m_geneSize;
  TGene* m_genePool;
  int (*m_evalGene) (char*); // given fitness function
  int m_generations;

  // settings
  int m_genesInGeneration; // gene pool size each generation
  int m_mutationRate; // how many bits to mutate in each mutation
};

#endif	/* GENETIC_H */

