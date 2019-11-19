#ifndef POINT_H
#define POINT_H
#include <QVector>
#include <QStringList>

//Defines the data
class Point
{
public:
    Point();
    Point(QStringList);
    double Class;
    QVector<double> Data;
};

#endif // POINT_H
