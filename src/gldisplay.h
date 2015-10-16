#ifndef GL_H
#define GL_H

#include "dtm.h"

#include <stdexcept>

#include <QVector>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

// Declare a class MainWindow to resolve circular inclusion
class MainWindow;


/* Window where OpenGL graphics will be done.
 *
 * Upon upgrading to Qt 5.4, adapt it to a QOpenGLWidget instead of a QWindow
 */
class glDisplay : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    // Exception raised during window initialization
    class initException : public std::runtime_error
    {
    public:
        initException(const char* message) : std::runtime_error(message) {}
    };

private:
    MainWindow * const m_mainW; // A pointer in order to communicate with main window
    QOpenGLContext * const m_context; // The context responsible for the drawing
    const QVector<Point>& m_vertices;

protected:
    QSize m_windowSize;
    bool m_initOK; // OpenGL has (or not) been initialized

public:
    explicit glDisplay(MainWindow *mainW, const QVector<Point>& vertices);
    ~glDisplay();

    // Contains initialization code which cannot be executed before the window has been shown
    virtual void initialize();


public slots:
    void render(); // drawing function
    void reshapeWindow(int width, int height);

protected:
    // Function answering all events
    bool event(QEvent * event);
    // Function answering the ExposureEvent: called when the window exposure changes
    void exposeEvent(QExposeEvent * event);
};

#endif // GL_H
