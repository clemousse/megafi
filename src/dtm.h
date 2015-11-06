#ifndef DTM_H
#define DTM_H

#include <QVector>
#include <QString>

#include <QGLViewer/vec.h>

class DTM {
    // Data types
protected:
    struct Point {
        double x,y,z;
    };


    // Members
private:
    QVector<Point> m_vertices; // The vertices array

    // data bounding rectangle
    Point m_dataSizeMin;
    Point m_dataSizeMax;

    long m_lineLength;
    long m_nbLines;

public:
    DTM();
    DTM(const QString& filePath);
    virtual ~DTM();

    qglviewer::Vec getLL();
    qglviewer::Vec getUR();

private:
    bool readDTM(const QString& path);
    void computeDataSize();
    void computeLineLength();
};




#endif // DTM_H
