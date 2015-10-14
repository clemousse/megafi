#ifndef GL_H
#define GL_H

#include <QWindow>


class MainWindow;

/* Window where OpenGL graphics will be done.
 *
 * Upon upgrading to Qt 5.4, adapt it to a QOpenGLWidget instead of a QWindow
 */
class glDisplay : public QWindow
{
    Q_OBJECT

private:
    MainWindow * const m_mainW; // A pointer in order to communicate with main window

public:
    explicit glDisplay(MainWindow *mainW);
    ~glDisplay();
};

#endif // GL_H
