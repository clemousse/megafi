#include "gldisplay.h"

glDisplay::glDisplay(MainWindow * mainW, const QVector<Point> &vertices) :
    QGLViewer(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW),
    m_vertices(vertices), m_indices(),
    m_windowSize(400, 300),
    m_dataSizeMin(), m_dataSizeMax(),
    m_lineLength(1000)
{
    setBaseSize(m_windowSize);

    // dataset size
    computeDataSize();
}

glDisplay::~glDisplay()
{
}

void glDisplay::reshapeWindow(int width, int height)
{
    m_windowSize.setWidth(width);
    m_windowSize.setHeight(height);

    init();
}

void glDisplay::init()
{
    // draw axis and grid (does not work)
    setAxisIsDrawn(true);
    setGridIsDrawn(true);

    // set background and frontground colors
    QColor bg(0, 0, 0);
    setBackgroundColor(bg);
    QColor fg(255, 255, 255);
    setForegroundColor(fg);

    glViewport(0, 0, m_windowSize.width(), m_windowSize.height()); // Set the viewport size to fill the window

    // Move camera
    setSceneBoundingBox(m_dataSizeMin, m_dataSizeMax);
    showEntireScene();
}

#include <QDebug>

void glDisplay::draw()
{
    // I can begin to draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen
    m_indices.clear();

    glLineWidth(1);

    for(unsigned int i = m_lineLength ; i < static_cast<unsigned int>(m_vertices.length()) ; ++i)
    {
        if(i % m_lineLength)
        {
            m_indices.push_back(i);
            m_indices.push_back(i - m_lineLength);
            m_indices.push_back(i - m_lineLength);
            m_indices.push_back(i - m_lineLength -1);
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
    glDrawElements(GL_LINES, m_indices.length(), GL_UNSIGNED_INT, m_indices.constData());
    glDisableClientState(GL_VERTEX_ARRAY);
}

void glDisplay::computeDataSize()
{
    m_dataSizeMin.x = INFINITY;
    m_dataSizeMin.y = INFINITY;
    m_dataSizeMin.z = INFINITY;
    m_dataSizeMax.x = 0;
    m_dataSizeMax.y = 0;
    m_dataSizeMax.z = 0;

    for(long i = 0 ; i < m_vertices.length() ; ++i)
    {
        if(m_vertices[i].x < m_dataSizeMin.x) m_dataSizeMin.x = m_vertices[i].x;
        if(m_vertices[i].x > m_dataSizeMax.x) m_dataSizeMax.x = m_vertices[i].x;
        if(m_vertices[i].y < m_dataSizeMin.y) m_dataSizeMin.y = m_vertices[i].y;
        if(m_vertices[i].y > m_dataSizeMax.y) m_dataSizeMax.y = m_vertices[i].y;
        if(m_vertices[i].z < m_dataSizeMin.z) m_dataSizeMin.z = m_vertices[i].z;
        if(m_vertices[i].z > m_dataSizeMax.z) m_dataSizeMax.z = m_vertices[i].z;
    }

    qDebug() << "MNT is between (" << m_dataSizeMin.x << ',' << m_dataSizeMin.y << ',' << m_dataSizeMin.z
             << ") and (" << m_dataSizeMax.x << ',' << m_dataSizeMax.y << ',' << m_dataSizeMax.z << ")";
}

