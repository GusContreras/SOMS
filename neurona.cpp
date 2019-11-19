#include "neurona.h"

Neurona::Neurona()
{

}

Neurona::Neurona(int size)
{
    for(int i = 0; i < size; i++)
    {
        weights.append(Rand());
    }
}

double Neurona::distanciaHamming(QVector<double> x)
{
    int distancia = 0;
    for(int i = 0; i < x.length(); i++)
    {
        if(x[i] != weights[i])
        {
            distancia++;
        }
    }
    return distancia;
}

double Neurona::distanciaEuclidiana(QVector<double> x)
{
    double sumatoria = 0;
    for(int i = 0; i < x.length(); i++)
    {
        sumatoria += pow((x[i] - weights[i]),2);
    }
    return sqrt(sumatoria);
}

void Neurona::actualiza(QVector<double> x, QVector<double> g, double n)
{
    for(int i = 0; i < x.length(); i++)
    {
        weights[i] += n*(x[i] - g[i]);
    }
}

double Neurona::Rand()
{
    return (rand()/(double)RAND_MAX);
}
