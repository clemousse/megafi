#include "dtm.h"

#include <QFile>
#include <QDebug>


/* Purpose: read the coordinates from a DTM file and store them in a QVector
 * Arguments:
 *     path: path and filename of the file to be read
 *     vector: the vector in which the coordinates will be stored.
 *             It must have been defined by the user previously.
 *             The vector contains 'Point's (datatype), see dtm.h for the declaration of Point.
 * Return: void
 */
void readDTM(const QString& path, QVector<Point>& vector)
{
    // Opening the file
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Convert the file into a text stream (un flux de texte) for better readability
        QTextStream filestream(&file);

        // Declare variables to be used in the loop
        QString line;
        QStringList coordinates;
        Point p;

        while(!filestream.atEnd())
        {
            // The file is read line-by-line
            line = filestream.readLine();
            // A line contains three numbers; make one string (character sequence) for each number
            coordinates = line.split(' ');

            // Convert each string to a real machine number
            double a = coordinates[0].toDouble();
            double b = coordinates[1].toDouble();
            double c = coordinates[2].toDouble();

            // Store the coordinates into a Point structure
            p.x=a;
            p.y=b;
            p.z=c;

            // Add the three coordinates at the end of the vector
            vector.append(p);
        }

        // The file has been opened, close it now
        file.close();
    }
}
