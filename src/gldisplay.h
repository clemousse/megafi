#ifndef GL_H
#define GL_H

#include "flowpath.h"
#include "dtm.h"

#include <stdexcept>

#include <QList>

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/mouseGrabber.h>
#include <QGLViewer/manipulatedFrame.h>
#include <QMouseEvent>



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
    const MainWindow& m_mainW; // A pointer in order to communicate with main window
    const DTM* const *m_dtm;   // The data
    const QList<FlowPath>& m_flows;

protected:
    QSize m_windowSize;


public:
    glDisplay(const MainWindow& mainW, const DTM* const *data, const QList<FlowPath>& flows);
    ~glDisplay();

    // Contains initialization code which cannot be executed before the window has been shown
    virtual void init();

    void mousePressEvent(QMouseEvent* const event);

public slots:
    void draw(); // drawing function
    void reshapeWindow(int width, int height);

protected:
    // Drawing functions
    template <int primit> inline void draw_init();
    template <int primit> inline void draw_line(unsigned int i);
    template <int primit> inline void draw_back(unsigned int j);
    template <int primit> inline void draw_end();
    void draw_function(unsigned int);
    size_t array_size(const int primitive) const;

    void computeClickedIndex(const qglviewer::Vec& mouse_world);

public:
    class UnknownPrimitive : public std::runtime_error
    {
    public:
        UnknownPrimitive() : std::runtime_error("Unknown primitive") {}
        UnknownPrimitive(const int prim, std::string message = "")
            : std::runtime_error(std::string("Unknown primitive ")
                                 + std::to_string(prim)
                                 + (message.empty() ? "" : (std::string(" [") + message + std::string("]")))
                                 ) {}
    };
};



/*
#define GL_POINTS				0x0000     0b0000
#define GL_LINES				0x0001     0b0001
#define GL_LINE_LOOP				0x0002 0b0010
#define GL_LINE_STRIP				0x0003 0b0011
#define GL_TRIANGLES				0x0004 0b0100
#define GL_TRIANGLE_STRIP			0x0005 0b0101
#define GL_TRIANGLE_FAN				0x0006 0b0110
#define GL_QUADS				0x0007     0b0111
#define GL_QUAD_STRIP				0x0008 0b1000
#define GL_POLYGON				0x0009     0b1001
*/

// OpenGL mode than can be used
#define MODE_LEGACY         0
#define MODE_VERTEX_ARRAY   1
#define MODE_VERTEX_INDICES 2

// The primitive that can be used
#define PRIM_TRIANGLES      0b00000000
#define PRIM_SQUARES        0b00000001

// The design that can be used
#define THE_DESIGN          0b11111110
#define DESIGN_POINT        0b00000010
#define DESIGN_EDGE         0b00000100
#define DESIGN_SHAPE        0b00001000

// Other implemented primitives
#define PRIM__LINE_LOOP     0b00010000
#define PRIM__TRIANGLES     0b00100000

#define OPENGL(choice, dest) switch(choice) { \
    case DESIGN_POINT: \
        dest = GL_POINTS; break; \
    case DESIGN_EDGE | PRIM_TRIANGLES: \
        dest = GL_LINE_STRIP; break; \
    case DESIGN_EDGE | PRIM_SQUARES: \
        dest = GL_LINES; break; \
    case DESIGN_SHAPE | PRIM_TRIANGLES: \
        dest = GL_TRIANGLE_STRIP; break; \
    case DESIGN_SHAPE | PRIM_SQUARES: \
        dest = GL_QUAD_STRIP; break; \
    case PRIM__LINE_LOOP: \
        dest = GL_LINE_LOOP; break; \
    case PRIM__TRIANGLES: \
        dest = GL_TRIANGLES; break; \
    default: \
        throw new UnknownPrimitive(choice); \
}

// FINAL CHOICE
#define MODE MODE_VERTEX_ARRAY
#define PRIM DESIGN_EDGE | PRIM_SQUARES

#endif // GL_H
