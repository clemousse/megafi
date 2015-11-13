#include "gldisplay.h"
#include "gldisplay.inl"
#include "mainwindow.h"

#include <QDebug>

glDisplay::glDisplay(MainWindow& mainW,
                     const megafi::DTM* const* dtm,
                     const QList<const megafi::FlowPath*>& flows)
    : m_mainW(mainW),
    m_dtm(dtm),
    m_flows(flows),
    m_windowSize(400, 300),
    m_departureSelection(false)

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

void glDisplay::beginDraw()
{
    if(isVisible())
    {
        init();
        draw();
    }
}

void glDisplay::draw()
{
    // I can begin to draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

    // States
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Building DTM
    if(*m_dtm)
        drawData<megafi::DTM>(**m_dtm);

    // For each flow
    for(QList<const megafi::FlowPath*>::const_iterator flow = m_flows.cbegin() ;
        flow != m_flows.cend() ;
        ++flow)
    {
        glLineWidth((*flow)->getLineWidth());
        drawData<megafi::FlowPath>(**flow);
    }

    // States
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#ifdef IMANE
    for(long i = 0 ; i < m_minIndices.length()-1 ; ++i)
    {

        glBegin(GL_LINES);
        glColor3d(1,0,0);
        const Point& p1 = m_vertices[m_minIndices[i]],
                p2 = m_vertices[m_minIndices[i+1]];

        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);

        glEnd();
    }
#endif
}


// in order to connect mousePressEvent if the selection mode is checked on mainwindow
void glDisplay::rbClick (bool chckD)
{
    m_departureSelection = chckD;
}


void glDisplay::mousePressEvent(QMouseEvent* const event)
{
    event->accept();

    if(m_departureSelection)
    {
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

            m_mainW.setClickedCoordinates(mouse_world);
            m_mainW.addFlow((*m_dtm)->computeIndex(mouse_world));
        }
        else
        {
            qDebug() << "Not found\n";
            qWarning () << "Warning, Warning, Warning\n";
            //qFatal() << "Fatal\n"; => bug !
            qCritical () << "Message critique, attention attention\n";

        }
    }
    else
    {
        //release the curseur of the mouse to the parent class
        QGLViewer::mousePressEvent(event);
    }
}


