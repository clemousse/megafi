#ifndef DTM_H
#define DTM_H

#include <QVector>
#include <QString>

struct Point {
    double x,y,z;
};

void readDTM(const QString& path, QVector<Point>& vector);



#endif // DTM_H
