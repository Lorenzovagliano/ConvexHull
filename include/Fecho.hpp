#ifndef FECHO_H
#define FECHO_H

#include "Ponto.hpp"

class Fecho{
    public:
        Ponto* pontos;
        int tamanho;

        Fecho(Ponto* _pontos);

        Ponto* JarvisFecho(Ponto _pontos[], long int _n, long int& _tamanhoFecho);
        Ponto* MergeConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho);
        Ponto* InsertConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho);
        Ponto* BucketConvexHullGraham(Ponto _pontos[], long int _n, long int& _tamanhoFecho);

};

#endif