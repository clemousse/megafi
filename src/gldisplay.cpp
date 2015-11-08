#include "gldisplay.h"
#include "mainwindow.h"
#include "dtm.h"
#include <QDebug>

glDisplay::glDisplay(MainWindow &mainW,
                     const DTM* const *data,
                     const QList<const FlowPath*> &flows)
    : m_mainW(mainW),
    m_dtm(data),
    m_flows(flows),
    m_windowSize(400, 300)
{
    setBaseSize(m_windowSize);

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


    glViewport(0, 0, m_windowSize.width(), m_windowSize.height()); // Set the viewport size to fill the window

    // Move camera
    if(*m_dtm)
    {
        setSceneBoundingBox((*m_dtm)->getLL(), (*m_dtm)->getUR());
        showEntireScene();
    }
}

void glDisplay::draw()
{
    // I can begin to draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

    // Building DTM
    glLineWidth(1);
    drawData<DTM>(**m_dtm);
}

template<class Child>
void glDisplay::drawData(const Child &data)
{
    switch(data.getMode())
    {
    case MODE_LEGACY:
    {
        Child& unconstData = const_cast<Child&>(data);
        data.buildLegacy(unconstData);
    }
        break;
    case MODE_VERTEX_ARRAY:
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, sizeof(qglviewer::Vec), data.getVertexArray());
        glDrawArrays(glPrimitive(data.getPrimitive()), 0, data.getArrayLength());
        glDisableClientState(GL_VERTEX_ARRAY);
        break;
    case MODE_VERTEX_INDICES:
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, sizeof(qglviewer::Vec), data.getVertices());
        glDrawElements(glPrimitive(data.getPrimitive()),
                       data.getArrayLength(),
                       GL_UNSIGNED_INT,
                       data.getIndiceArray());
        glDisableClientState(GL_VERTEX_ARRAY);
        break;
    }
}

void glDisplay::mousePressEvent(QMouseEvent* const event)
{
    event->accept();

    const qglviewer::Camera* const camera = this->camera();

    QPoint mouse_scr;
    mouse_scr.setX(event->x());
    mouse_scr.setY(event->y());

    qDebug () << "ecran x : " << mouse_scr.x() << endl
              << "ecran y : " << mouse_scr.y() << endl;
    bool found;
    const qglviewer::Vec mouse_world = camera->pointUnderPixel(mouse_scr, found);

    if(found)
    {
        qDebug () << "position x : " << mouse_world.x << endl
                  << "position y : " << mouse_world.y << endl
                  << "position z : " << mouse_world.z << endl;

        m_mainW.addFlow((*m_dtm)->computeIndex(mouse_world));
    }
    else
        qDebug() << "Not found";

    //release the curseur of the mouse to the parent class
    QGLViewer::mousePressEvent(event);


}
