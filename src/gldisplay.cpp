#include "gldisplay.h"

glDisplay::glDisplay(MainWindow * mainW, const QVector<Point> &vertices) :
    QGLViewer(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW),
    m_vertices(vertices),
    m_windowSize(400, 300)
{
    setBaseSize(m_windowSize);
}

glDisplay::~glDisplay()
{
}

void glDisplay::reshapeWindow(int width, int height)
{
    m_windowSize.setWidth(width);
    m_windowSize.setHeight(height);
}

void glDisplay::init()
{
    glClearColor(0, 0, 0, 255);
}

void glDisplay::draw()
{
    // I can begin to draw
    glViewport(0, 0, m_windowSize.width(), m_windowSize.height()); // Set the viewport size to fill the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen
    glNormal3f(1, 1, 1);

    glBegin(GL_TRIANGLES);
    glColor3ui(255, 255, 255);
    glVertex2d(0.5, 0.5);
    glVertex2d(0.5, -0.5);
    glVertex2d(-0.5, -0.5);
    glEnd();
}

