#ifndef RETA_H
#define RETA_H

#include <iostream>
#include "Ponto.hpp"


class Reta{
    public:
        Ponto p1;
        Ponto p2;

        Reta(Ponto _p1, Ponto _p2);
};

#endif