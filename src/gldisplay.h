#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include "flowpath.h"
#include "dtm.h"
#include "flowpathview.h"

#include <QList>
#include <QMouseEvent>
#include <QCursor>
#include <QFileDialog>
#include <QCloseEvent>

#include <QGLViewer/qglviewer.h>

namespace megafi
{

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
    QCursor m_selectCursor;
    QCursor m_moveCursor;

public:
    glDisplay(const megafi::DTM* const* dtm,
              const QList<const megafi::FlowPath *> *flows);
    ~glDisplay();

    void mousePressEvent(QMouseEvent* const event);

public slots:
    void show();
    void reinit();
    void reshapeWindow(int width, int height);
    void rbClick(bool chckD);
    void saveSnapshot();


signals:
    void windowHasChanged() const;
    void clicked(qglviewer::Vec) const;
    void visible(bool) const;
    void opened() const;
    void closed() const;

private:
    void init(); // Contains initialization code which cannot be executed before the window has been shown
    void draw(); // Drawing function
    template<class Datatype> void drawData(const Datatype&) const;
    void closeEvent(QCloseEvent *) override;
};

}

#endif // GLDISPLAY_H
