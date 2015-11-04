#include "gldisplay.h"
#include <QDebug>

glDisplay::glDisplay(MainWindow * mainW, const QVector<Point> &vertices) :
    QGLViewer(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW),
    m_vertices(vertices),
    m_windowSize(400, 300),
    m_dataSizeMin(), m_dataSizeMax(),
    m_lineLength(4000),
    //m_min_vertices(vertices),
    m_minIndices()
{
    setBaseSize(m_windowSize);

    // dataset size
    computeDataSize();
    lin ();

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
    setSceneBoundingBox(m_dataSizeMin, m_dataSizeMax);
    showEntireScene();
}

void glDisplay::draw()
{
    // I can begin to drawgl_quad_strip
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

    glLineWidth(1);

    for(long i = m_lineLength ; i < m_vertices.length() ; ++i)
    {
        if(i % m_lineLength)
        {
            //glBegin(GL_LINE_STRIP);
            const Point& p1 = m_vertices[i-1],
                    p2 = m_vertices[i - m_lineLength -1],
                    p3 = m_vertices[i],
                    p4 = m_vertices[i-m_lineLength];
            //glEnd();

            glBegin(GL_QUADS);
            glColor3d (255, 0, 255);
                glVertex3d(p1.x, p1.y, p1.z);
                glVertex3d(p2.x, p2.y, p2.z);
                glVertex3d(p3.x, p3.y, p3.z);
                glVertex3d(p4.x, p4.y, p4.z);
            glEnd();

            glBegin(GL_LINE_STRIP);
            glColor3d (255, 255, 255);
            glLineWidth(10);
                glVertex3d(p3.x, p3.y, p3.z);
                glVertex3d(p4.x, p4.y, p4.z);
                //glVertex3d(p1.x, p1.y, p1.z);
                glVertex3d(p2.x, p2.y, p2.z);

            glEnd();

        }
    }
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


void glDisplay::lin()
{
    m_lineLength=0;
    for(long i = 0; i < m_vertices.length()-1; ++i)
    {
         if (m_vertices[i].y == m_vertices[i+1].y)
            {
             m_lineLength++;
            }
        else
            {

             qDebug() << "La longueur de la ligne est " << m_lineLength << "points";
             m_lineLength=0;


            }
     }

}


void glDisplay::PointN()
{
    /*
     *  Il faut stocker les INDICES des points minimaux :
     *      m_minIndices[0] contiendra l'indice du point de départ. Le point de départ sera alors m_vertices[m_minIndices[0]]
     *      À la fin des quatre tests, m_minIndices[1] contiendra l'indice du point suivant.
     *      On recommence avec m_minIndices[1].
     */
    long currentStep = 0;

    do
    {

        long i = m_minIndices[currentStep];


        if((i%m_lineLength!=0) && (m_vertices[i-1].z < m_vertices[i].z))
            m_minIndices.push_back(i-1);
        if((i%m_lineLength!=3999)&& (m_vertices[i+1].z < m_vertices[i].z))
            m_minIndices.push_back(i+1);
        if((i>((m_vertices.length()-1)-(m_lineLength))) && (m_vertices[i-m_lineLength].z < m_vertices[i].z))
            m_minIndices.push_back(i-m_lineLength);
        if((i<=3999) && (m_vertices[i+m_lineLength].z < m_vertices[i].z))
            m_minIndices.push_back(i+m_lineLength);
        qDebug() << "le point suivant est avec les coordonnnées suivantes:  z=" << m_vertices[i].z << ',y=' << m_vertices[i].y <<','<< m_vertices[i].x ;
        ++currentStep;

    } while(m_minIndices[currentStep-1] == m_minIndices[currentStep]);



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
    {
        qDebug () << "position x : " << mouse_world.x << endl
                  << "position y : " << mouse_world.y << endl
                  << "position z : " << mouse_world.z << endl;
        // Retrouver l'indice du point qui a pour coordonnées mouse_world

        for(long i = 0 ; i < m_lineLength-1; i+=1)
        {
            if (m_vertices[i].x == mouse_world.x )
            {
                m_minIndices.push_back(i);

            }
            else if (((m_vertices[i].x < mouse_world.x ) && (mouse_world.x< m_vertices[i+1].x)) && ((m_vertices[i+1].x)-(mouse_world.x) < (mouse_world.x)-(m_vertices[i].x)))

            {
                m_minIndices.push_back(i+1);

            }
            else if (((m_vertices[i].x < mouse_world.x ) && (mouse_world.x < m_vertices[i+1].x)) && ((m_vertices[i+1].x)-(mouse_world.x) > (mouse_world.x)-(m_vertices[i].x)))

            {
                m_minIndices.push_back(i);

            }

        }
        for(long j = m_minIndices[0] ; j < m_vertices.length()-m_lineLength; j+=m_lineLength)
        {
            if (m_vertices[j].y == mouse_world.y )
            {
                m_minIndices[0]=j;


            }
            else if (((m_vertices[j].y < mouse_world.y ) && (mouse_world.y< m_vertices[j+m_lineLength].y)) && ((m_vertices[j+m_lineLength].x)-(mouse_world.y) < (mouse_world.y)-(m_vertices[j].y)))

            {
                m_minIndices[0]=j+m_lineLength;

            }
            else if (((m_vertices[j].y < mouse_world.y ) && (mouse_world.y < m_vertices[j+m_lineLength].y)) && ((m_vertices[j+m_lineLength].y)-(mouse_world.y) > (mouse_world.y)-(m_vertices[j].y)))

            {
                m_minIndices[0]=j;

            }


        }

           PointN();
    }
    else
        qDebug() << "Not found";

    //release the curseur of the mouse to the parent class
    QGLViewer::mousePressEvent(event);


}
