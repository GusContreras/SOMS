#include "point.h"

Point::Point()
{

}

Point::Point(QStringList words)
{
    for(int i = 0; i < words.length()-1; i++){
        Data.append(words[i].toDouble());
    }

    Class = words.last().toDouble();
}
