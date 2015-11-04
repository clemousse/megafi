#include "gldisplay.h"
#include "draw.h"
#include <QDebug>

glDisplay::glDisplay(MainWindow * mainW, const QVector<Point> &vertices) :
    QGLViewer(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW),
    m_vertices(vertices), m_indices(),
    m_windowSize(400, 300),
    m_dataSizeMin(), m_dataSizeMax(),
    m_lineLength(7)
{
    setBaseSize(m_windowSize);

    // dataset size
    computeDataSize();

    //In order to make MouseGrabber react to mouse events
    setMouseTracking(true);
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

    glLineWidth(1);

    glViewport(0, 0, m_windowSize.width(), m_windowSize.height()); // Set the viewport size to fill the window

    // Move camera
    setSceneBoundingBox(m_dataSizeMin, m_dataSizeMax);
    showEntireScene();
}


inline void glDisplay::draw_function(unsigned int index) {
#if MODE == MODE_VERTEX_INDICES
    m_indices.push_back(index);
#else
    glVertex3d(m_vertices[index].x, m_vertices[index].y, m_vertices[index].z);
#endif
}

void glDisplay::draw()
{
    // I can begin to draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

#if MODE == MODE_VERTEX_INDICES
    m_indices.clear();
#endif

    draw_init<PRIM>();

    for(unsigned int i = 0 ; i < static_cast<unsigned int>(m_vertices.length()) ; ++i)
    {
        if(i % m_lineLength)
            draw_line<PRIM>(i);
        else
            draw_beginline<PRIM>(i);
    }

    draw_end<PRIM>();

#if MODE == MODE_VERTEX_INDICES
    int glprim = 0;
    OPENGL(PRIM, glprim)
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
    glDrawElements(glprim, m_indices.length(), GL_UNSIGNED_INT, m_indices.constData());
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
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


void glDisplay::mousePressEvent(QMouseEvent* const event)
{
    event->accept();

    const qglviewer::Camera* const camera = this->camera();

    //const float z = 1;

    QPoint mouse_scr;
    mouse_scr.setX(event->x());
    mouse_scr.setY(event->y());

    qDebug () << "ecran x : " << mouse_scr.x() << endl
              << "ecran y : " << mouse_scr.y() << endl;
    bool found;
    const qglviewer::Vec mouse_world = camera->pointUnderPixel(mouse_scr, found);

    if(found)
        qDebug () << "position x : " << mouse_world.x << endl
                  << "position y : " << mouse_world.y << endl
                  << "position z : " << mouse_world.z << endl;
    else
        qDebug() << "Not found";

    //release the curseur of the mouse to the parent class
    QGLViewer::mousePressEvent(event);
}





