#ifndef NEURONA_H
#define NEURONA_H

#include <QtMath>
#include <QVector>
#include <cstdlib>
#include <ctime>
#include <QRandomGenerator>

class Neurona
{
public:
    Neurona();
    Neurona(int size);
    double distanciaHamming(QVector<double> x);
    double distanciaEuclidiana(QVector<double> x);
    void actualiza(QVector<double> x, QVector<double> g, double n);
    double Rand();

    QVector<double> weights;
};

#endif // NEURONA_H
