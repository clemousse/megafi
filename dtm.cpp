#include "dtm.h"
#include <QFile>
#include <QList>
#include <QDebug>
using namespace std;
QVector<Point> readDTM(QString path)
{
    QFile file(path);
    bool resultat;
    resultat = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (resultat)
    {
        QTextStream filestream(&file);
        QString line;
        QStringList nb;

        while(!filestream.atEnd())
        {
            line=filestream.readLine();
            //qDebug() << "Ligne lue : " << line;
            nb=line.split(' ');
            for (int i=0;i<3;i++)
            {
                nb[i];
                //qDebug() << "Element " << i << " est " << nb[i] << endl;
                double a = nb[i].toDouble();
                //qDebug() << "l element de la liste numero "<<i<< " est:"<<a<<endl;

            }
        }
        file.close();
    }
}
