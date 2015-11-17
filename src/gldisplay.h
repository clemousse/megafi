#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include "flowpath.h"
#include "dtm.h"
#include "flowpathview.h"

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
    const megafi::DTM *const *const m_dtm;  // The data
    const QList<const megafi::FlowPath*>* const m_flows;

    bool m_initialized;


protected:
    QSize m_windowSize;
    bool m_departureSelection;

public:
    glDisplay(const MainWindow *mainW,
              const megafi::DTM* const* dtm,
              const QList<const megafi::FlowPath *> *flows);
    ~glDisplay();

    void mousePressEvent(QMouseEvent* const event);

public slots:
    void reinit();
    void reshapeWindow(int width, int height);
    void rbClick(bool chckD);

signals:
    void windowHasChanged() const;
    void clicked(qglviewer::Vec) const;

private:
    void init(); // Contains initialization code which cannot be executed before the window has been shown
    void draw(); // Drawing function
    template<class Datatype> void drawData(const Datatype&) const;
};

#endif // GLDISPLAY_H
