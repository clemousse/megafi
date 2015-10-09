#include "dtm.h"

#include <QFile>
#include <QDebug>


QVector<Point> readDTM(const QString &path)
{
    QFile file(path);
    bool resultat;
    resultat = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (resultat)
    {
        QTextStream filestream(&file);
        QString line;
        QStringList coordinates;

        while(!filestream.atEnd())
        {
            line = filestream.readLine();
            coordinates = line.split(' ');
            for (int i=0 ; i<3 ; i++)
            {
                double a = coordinates[i].toDouble();
            }
        }
        file.close();
    }
}
