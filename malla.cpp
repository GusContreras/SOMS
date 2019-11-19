#include "malla.h"

Malla::Malla()
{

}

void Malla::inicializa(int size, int patron)
{
    for(int i = 0; i < size*size; i++)
    {
        Neurona ne = Neurona(patron);
        //neuronas.append(ne);
    }
}

