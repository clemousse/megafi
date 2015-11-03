#ifndef GL_H
#define GL_H

#include "dtm.h"

#include <stdexcept>

#include <QGLViewer/qglviewer.h>


// Declare a class MainWindow to resolve circular inclusion
class MainWindow;


/* Window where OpenGL graphics will be done.
 *
 * Upon upgrading to Qt 5.4, adapt it to a QOpenGLWidget instead of a QWindow
 */
class glDisplay : public QGLViewer
{
    Q_OBJECT

private:
    MainWindow * const m_mainW; // A pointer in order to communicate with main window
    const QVector<Point>& m_vertices;
    QVector<unsigned int> m_indices;

protected:
    QSize m_windowSize;

    // data bounding rectangle
    qglviewer::Vec m_dataSizeMin;
    qglviewer::Vec m_dataSizeMax;

    long m_lineLength;

public:
    explicit glDisplay(MainWindow *mainW, const QVector<Point>& vertices);
    ~glDisplay();

    // Contains initialization code which cannot be executed before the window has been shown
    virtual void init();


public slots:
    void draw(); // drawing function
    void reshapeWindow(int width, int height);
    void computeDataSize();
};

#endif // GL_H
