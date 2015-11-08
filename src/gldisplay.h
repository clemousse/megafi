#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include "flowpath.h"
#include "dtm.h"

#include <QList>
#include <QMouseEvent>

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
    MainWindow& m_mainW;        // A pointer in order to communicate with main window
    const megafi::DTM* const* const m_dtm;  // The data
    const QList<const megafi::FlowPath*>& m_flows;


protected:
    QSize m_windowSize;
    bool m_departureSelection;

public:
    glDisplay(MainWindow& mainW,
              const megafi::DTM* const* dtm,
              const QList<const megafi::FlowPath*>& flows);
    ~glDisplay();

    void mousePressEvent(QMouseEvent* const event);

public slots:
    void beginDraw();
    void reshapeWindow(int width, int height);
    void rbClick(bool chckD);

signals:
    void needsRebuild() const;

private:
    void init(); // Contains initialization code which cannot be executed before the window has been shown
    void draw(); // drawing function
    template<class Datatype>
    void drawData(const Datatype&) const;
};

#endif // GLDISPLAY_H
