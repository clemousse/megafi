#ifndef DTM_H
#define DTM_H

#include <QVector>
#include <QString>

struct Point {
    double x,y,z;
};

QVector<Point> readDTM(const QString& path);



#endif // DTM_H
