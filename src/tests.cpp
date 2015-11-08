#include "tests.h"

#include <QGLViewer/vec.h>

#include <QFile>
#include <QDebug>

bool runTests()
{
    testOnreadDTM(TEST_DTM_FILE);
    return true;
}

void testOnreadDTM(const QString& dtmFileName)
{
    const megafi::DTM testObj(dtmFileName);
    const qglviewer::Vec* const result = testObj.getVertices();

    if (result[51].x == 901275 && result[51].y == 1944975 && result[51].z == 1228.50)
    {
        qDebug()  << "OK - La 50ème ligne du tableau contient bien X = " << result[51].x
                  << " Y = " << result[51].y
                  << " Z = " << result[51].z << endl;
    }
    else
    {
        qDebug()  << "Le tableau ne correspond pas au fichier d'entrée" << endl;
    }

    //dernière ligne : 999975.00 1900025.00 515.20
    if (result[7195999].x == 999975 && result[7195999].y  == 1900025 && result[7195999].z == 515.2)
    {
        qDebug()  << "OK - La dernière ligne du tableau contient bien X = " << result[7195999].x
                  << " Y = " << result[7195999].y
                  << " Z = " << result[7195999].z << endl;
    }

    else
     {
        qDebug()  << "Le tableau ne correspond pas au fichier d'entrée" << endl;
     }
}
