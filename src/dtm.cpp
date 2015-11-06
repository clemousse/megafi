#include "dtm.h"

#include <QFile>
#include <QDebug>


DTM::DTM()
    : m_vertices(),
      m_dataSizeMin(), m_dataSizeMax(),
      m_lineLength(0),
      m_nbLines(0)
{
}

DTM::DTM(const QString &filePath)
    : m_vertices(),
      m_dataSizeMin(), m_dataSizeMax(),
      m_lineLength(0),
      m_nbLines(0)
{
    if(readDTM(filePath))
    {
        computeDataSize();
        computeLineLength();
    }
}

qglviewer::Vec DTM::getLL()
{
    qglviewer::Vec p;
    p.x = m_dataSizeMin.x;
    p.y = m_dataSizeMin.y;
    p.z = m_dataSizeMin.z;
    return p;
}

qglviewer::Vec DTM::getUR()
{
    qglviewer::Vec p;
    p.x = m_dataSizeMax.x;
    p.y = m_dataSizeMax.y;
    p.z = m_dataSizeMax.z;
    return p;
}

/* Purpose: read the coordinates from a DTM file and store them in a QVector
 * Arguments:
 *     filePath: path and filename of the file to be read
 * Return: true if the file have been opened
 */
bool DTM::readDTM(const QString& path)
{
    bool bOpen = false;

    // Opening the file
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bOpen = true;

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
            m_vertices.append(p);
        }

        // The file has been opened, close it now
        file.close();
    }

    return bOpen;
}

void DTM::computeDataSize()
{
    m_dataSizeMin.x = INFINITY;
    m_dataSizeMin.y = INFINITY;
    m_dataSizeMin.z = INFINITY;
    m_dataSizeMax.x = 0;
    m_dataSizeMax.y = 0;
    m_dataSizeMax.z = 0;


    for(long i = 0 ; i < m_vertices.length() ; ++i)
    {
        if(m_vertices[i].x < m_dataSizeMin.x) m_dataSizeMin.x = m_vertices[i].x;
        if(m_vertices[i].x > m_dataSizeMax.x) m_dataSizeMax.x = m_vertices[i].x;
        if(m_vertices[i].y < m_dataSizeMin.y) m_dataSizeMin.y = m_vertices[i].y;
        if(m_vertices[i].y > m_dataSizeMax.y) m_dataSizeMax.y = m_vertices[i].y;
        if(m_vertices[i].z < m_dataSizeMin.z) m_dataSizeMin.z = m_vertices[i].z;
        if(m_vertices[i].z > m_dataSizeMax.z) m_dataSizeMax.z = m_vertices[i].z;
    }

    qDebug() << "MNT is between (" << m_dataSizeMin.x << ',' << m_dataSizeMin.y << ',' << m_dataSizeMin.z
             << ") and (" << m_dataSizeMax.x << ',' << m_dataSizeMax.y << ',' << m_dataSizeMax.z << ")";
}

void DTM::computeLineLength()
{
    const long NOT_COMPUTED = 0;
    long lineLength_prec = NOT_COMPUTED;
    m_lineLength = 1;
    m_nbLines = 1;

    // at each loop, m_lineLenght takes 1 if "m_vertices[i].y == m_vertices[i+1].y".
    // m_lineLenght begin at 1.
    for(long i = 0 ; i < m_vertices.length()-1; ++i, ++m_lineLength)
    {
          // when "m_vertices[i].y != m_vertices[i+1].y", we compare m_lineLenght and lineLenght_prec
          // to see if they have the same number of points.
          if (m_vertices[i].y != m_vertices[i+1].y)
          {
             if(lineLength_prec != NOT_COMPUTED && m_lineLength != lineLength_prec)
                 qWarning() << "Lines" << i << "and" << i+1 << "don't have the same length.";

             lineLength_prec = m_lineLength;
             m_lineLength = 0;
             ++m_nbLines;
          }
    }

    m_lineLength = lineLength_prec;
    qDebug() << "Lines are" << m_lineLength << "points long.";
}
