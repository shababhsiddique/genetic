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
#include <cstdlib>
#include "time.h"

Genetic::Genetic( char* geneSeed, int geneSize, int (*evalGene) (char*) )
{
  m_evalGene = evalGene;
  m_geneSize = geneSize;
  m_genesInGeneration = 10; // 10 different genes in generation
  m_mutationRate = 3; // how many bits to zap each generation? Consider Hamming walls.
  m_genePool = (TGene*)malloc(m_genesInGeneration*sizeof(TGene));
  for(int i=0; i<m_genesInGeneration; i++)
  {
    m_genePool[i].gene = (char*)malloc(m_geneSize*sizeof(char));
  }
  CopyGene( geneSeed, m_genePool[0].gene );
  m_genePool[0].score = m_evalGene(geneSeed);
  m_generations = 0;
  int randomSeed;
  srand(randomSeed+time(NULL));
  // populate first generation (all equal seed)
  CopyBestToRest();
};

Genetic::~Genetic()
{
  for(int i=0; i<m_genesInGeneration; i++)
  {
    free(m_genePool[i].gene);
  }
  free(m_genePool);
}

void Genetic::CopyGene( char* src, char* dest )
{
  if( m_geneSize > 0 )
  {
    for( int i=0; i<m_geneSize; i++ )
    {
      dest[i] = src[i];
    }
  }
}

void Genetic::CopyGene( TGene& src, TGene& dest )
{
    CopyGene( src.gene, dest.gene );
    dest.score = src.score;
}

void Genetic::CopyBestToRest()
{
  for(int i=1; i<m_genesInGeneration; i++)
  {
    CopyGene( m_genePool[0].gene, m_genePool[i].gene );
    m_genePool[i].score = m_genePool[0].score;
  }
}

void Genetic::Generation( int count )
{
  for( int i=0; i<count; i++ )
  {
    CopyBestToRest();
    Mutate();
    Sort();
    ++m_generations;
  }
}

char* Genetic::GetBestGene()
{
  return m_genePool[0].gene;
}

int Genetic::GetBestScore()
{
  return m_genePool[0].score;
}

int Genetic::GetGenerationCount()
{
  return m_generations;
}

// to skip modulo bias
int Genetic::GetRandom( int max )
{
  int validCeiling = (RAND_MAX - (RAND_MAX % max));
  int number;
  do
  {
    number = rand();
  }
  while( number > validCeiling );
  number = number % max;
  return number;
}

void Genetic::Mutate()
{
  if( (m_generations % 1000000) == 0 )
  {
      int whichByte = GetRandom(m_geneSize);
      m_genePool[0].gene[whichByte] = 0;
      m_genePool[0].score = m_evalGene(m_genePool[0].gene);
  }
  // skip the best one, star from 1
  for( int i=1; i<m_genesInGeneration; i++)
  {
    for( int rate=0; rate<m_mutationRate; rate++)
    {
      // flip random bit in random byte
      int whichByte = GetRandom(m_geneSize);
      int whichBit = GetRandom(8);
      char gammaray = 1 << whichBit; // cool name for mutagen, huh? :)
      m_genePool[i].gene[whichByte] ^= gammaray; // gets radiated
      m_genePool[i].score = m_evalGene(m_genePool[i].gene);
    }
  }
}

void Genetic::Sort()
{
  // just simple selection sort
  TGene temp;
  temp.gene = (char*)malloc(m_geneSize*sizeof(char*));
  for( int i=0; i<m_genesInGeneration; i++)
  {
    for( int j=i+1; j<m_genesInGeneration; j++)
    {
      if( m_genePool[j].score > m_genePool[i].score )
      {
        CopyGene( m_genePool[i], temp );
	CopyGene( m_genePool[j], m_genePool[i] );
        CopyGene( temp, m_genePool[j] );
      }
    }
  }
  free(temp.gene);
}
