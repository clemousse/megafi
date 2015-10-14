#include "tests.h"

#include <QFile>
#include <QDebug>

void testOnreadDTM(QVector<Point>& m_vector)
{

if (m_vector[51].x == 901275 && m_vector[51].y == 1944975 && m_vector[51].z == 1228.50)
    {
        qDebug()  << "OK - La 50ème ligne du tableau contient bien X = " << m_vector[51].x << " Y = " << m_vector[51].y << " Z = " << m_vector[51].z << endl;
    }
else
    {
        qDebug()  << "Le tableau ne correspond pas au fichier d'entrée" << endl;
    }

//dernière ligne : 999975.00 1900025.00 515.20
if (m_vector[7195999].x == 999975 && m_vector[7195999].y  == 1900025 && m_vector[7195999].z == 515.2)
    {
        qDebug()  << "OK - La dernière ligne du tableau contient bien X = " << m_vector[7195999].x << " Y = " << m_vector[7195999].y << " Z = " << m_vector[7195999].z << endl;
    }

else
     {
        qDebug()  << "Le tableau ne correspond pas au fichier d'entrée" << endl;
     }
}
