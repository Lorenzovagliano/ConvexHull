#ifndef FECHO_H
#define FECHO_H

#include <cmath>
#include "Ponto.hpp"

class Fecho{
    public:
        Ponto* pontos;
        int tamanho;

        Fecho(Ponto* _pontos);

        Ponto* JarvisFecho(Ponto _pontos[], int _n, int& _tamanhoFecho);
        Ponto* MergeConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);
        Ponto* InsertConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);
        Ponto* BucketConvexHullGraham(Ponto _pontos[], int _n, int& _tamanhoFecho);

};

#endif