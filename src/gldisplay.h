#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include "flowpath.h"
#include "dtm.h"

#include <QList>
#include <QMouseEvent>

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/mouseGrabber.h>
#include <QGLViewer/manipulatedFrame.h>



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
    MainWindow& m_mainW;      // A pointer in order to communicate with main window
    const DTM* const *m_dtm;        // The data
    const QList<const FlowPath*>& m_flows;

protected:
    QSize m_windowSize;


public:
    glDisplay(MainWindow& mainW, const DTM* const *data, const QList<const FlowPath*>& flows);
    ~glDisplay();

    // Contains initialization code which cannot be executed before the window has been shown
    virtual void init();

    void mousePressEvent(QMouseEvent* const event);

public slots:
    void draw(); // drawing function
    void reshapeWindow(int width, int height);

private:
    template<class Child>
    void drawData(const Child&);
};

#endif // GLDISPLAY_H
