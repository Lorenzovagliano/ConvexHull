#ifndef FECHO_H
#define FECHO_H

#include <cmath>
#include "Ponto.hpp"

class Fecho{
    public:
        Ponto* pontos;
        int tamanho;

        Fecho(Ponto* _pontos);

        // Encontra o fecho convexo de um conjunto de pontos usando o algoritmo de Jarvis
        Ponto* JarvisFecho(Ponto _pontos[], int _n, int& _tamanhoFecho);

        // Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Merge Sort
        Ponto* MergeConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);

        // Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Inserion Sort
        Ponto* InsertConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);

        // Encontra o fecho convexo de um conjunto de pontos usando o Scan de Graham com Bucket Sort
        Ponto* BucketConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);

};

#endif