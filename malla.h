#ifndef MALLA_H
#define MALLA_H

#include "neurona.h"

class Malla
{
public:
    Malla();

    void inicializa(int size, int patron);

    QVector<Neurona> neuronas;
};

#endif // MALLA_H
