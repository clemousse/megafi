#include "gldisplay.h"
#include "draw.h"
#include <QDebug>

glDisplay::glDisplay(const MainWindow& mainW, const DTM* const *data, const QList<FlowPath>& flows) :
    QGLViewer(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW),
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


inline void glDisplay::draw_function(unsigned int index) {
#if MODE == MODE_VERTEX_ARRAY
    m_pointsToDraw.push_back(m_vertices[index]);
#elif MODE == MODE_VERTEX_INDICES
    m_indices.push_back(index);
#else
    glVertex3d(m_vertices[index].x, m_vertices[index].y, m_vertices[index].z);
#endif
}

size_t glDisplay::array_size(const int primitive) const {
    const size_t nbPoints = static_cast<size_t>(m_vertices.length());
    size_t res = 0;

    if(primitive & DESIGN_POINT)
        res = nbPoints;
    else if(primitive & DESIGN_EDGE) {
        res = 2*nbPoints - m_lineLength;
        if(primitive & PRIM_TRIANGLES)
            res += m_nbLines * m_lineLength;
        else if(primitive & PRIM_SQUARES)
            res += m_nbLines * 2*(m_lineLength -1);
        else
            throw new UnknownPrimitive(primitive);
    }
    else if(primitive & DESIGN_SHAPE)
        res = 2*nbPoints;
    else if(primitive & PRIM__LINE_LOOP)
        res = 3*nbPoints - m_nbLines - m_lineLength;
    else if(primitive & PRIM__TRIANGLES)
        res = 6*nbPoints - m_nbLines - m_lineLength;
    else
        throw new UnknownPrimitive(primitive);

    return res;
}

void glDisplay::draw()
{
    glLineWidth(1);
    // I can begin to draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

#if MODE == MODE_VERTEX_ARRAY
    m_pointsToDraw.clear();
    try {
        m_pointsToDraw.reserve(array_size(PRIM));
    } catch(std::bad_alloc) {
        return;
    }
#elif MODE == MODE_VERTEX_INDICES
    try {
        m_indices.reserve(array_size(PRIM));
    } catch(std::bad_alloc) {
        return;
    }
#endif

    draw_init<PRIM>();

    const unsigned int vLength = static_cast<unsigned int>(m_vertices.length());
    for(unsigned int i = 0 ; i < vLength ; ++i)
    {
        if(i % m_lineLength == 0 // beginning of line
                && (PRIM) & DESIGN_EDGE) { // only if drawing edges
            for(unsigned int j = i + m_lineLength-1 ; j >= i && j < vLength ; --j)
                //                                           ^ security: j is unsigned so 0 - 1 >= 0
                draw_back<PRIM>(j);
        }

        draw_line<(PRIM) & THE_DESIGN>(i);
    }

    draw_end<PRIM>();

#if MODE != MODE_LEGACY
    int glprim = 0;
    OPENGL(PRIM, glprim)
#endif
#if MODE == MODE_VERTEX_ARRAY
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, sizeof(Point), m_pointsToDraw.data());
    glDrawArrays(glprim, 0, m_pointsToDraw.size());
    glDisableClientState(GL_VERTEX_ARRAY);
#elif MODE == MODE_VERTEX_INDICES
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, sizeof(Point), m_vertices.constData());
    glDrawElements(glprim, m_indices.length(), GL_UNSIGNED_INT, m_indices.constData());
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

void glDisplay::computePath()
{
    /*
     *
     * Il faut stocker les INDICES des points minimaux :
     *      m_minIndices[0] contiendra l'indice du point de départ. Le point de départ sera alors m_vertices[m_minIndices[0]]
     *      À la fin des quatre tests, m_minIndices[1] contiendra l'indice du point suivant.
     *      On recommence avec m_minIndices[1].
     */
    do
    {
        long i = m_minIndices.last();

        if( i%m_lineLength != 0
                && m_vertices[i-1].z < m_vertices[i].z )
            m_minIndices.push_back(i-1);

        else if( i%m_lineLength != m_lineLength-1
                 && m_vertices[i+1].z < m_vertices[i].z)
            m_minIndices.push_back(i+1);

        else if( i >= m_lineLength
                 && m_vertices[i-m_lineLength].z < m_vertices[i].z)
            m_minIndices.push_back(i-m_lineLength);

        else if( i < m_vertices.length()-m_lineLength
                 && m_vertices[i+m_lineLength].z < m_vertices[i].z)
            m_minIndices.push_back(i+m_lineLength);

        else
        {
            qDebug() << "End flow path";
            break;
        }

        qDebug() << "Next point's coordinates : x=" << m_vertices[i].x
                 << ", y=" << m_vertices[i].y
                 << ", z=" << m_vertices[i].z;
    } while(true);
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

        computeClickedIndex(mouse_world);
        computePath();
    }
    else
        qDebug() << "Not found";

    //release the curseur of the mouse to the parent class
    QGLViewer::mousePressEvent(event);


}

void glDisplay::computeClickedIndex(const qglviewer::Vec& mouse_world) {
    // Retrouver l'indice du point qui a pour coordonnées mouse_world
    for(long i = 0 ; i < m_lineLength-1; i+=1)
    {
        if (m_vertices[i].x == mouse_world.x)
        {
            m_minIndices.push_back(i);
        }
        else if ( (m_vertices[i].x < mouse_world.x) && (mouse_world.x < m_vertices[i+1].x) )
        {
            if(m_vertices[i+1].x - mouse_world.x < mouse_world.x - m_vertices[i].x)
                m_minIndices.push_back(i+1);
            else if (m_vertices[i+1].x - mouse_world.x > mouse_world.x - m_vertices[i].x)
                m_minIndices.push_back(i);
        }
    }

    for(long j = m_minIndices[0] ; j < m_vertices.length()-m_lineLength; j+=m_lineLength)
    {
        if (m_vertices[j].y == mouse_world.y )
        {
            m_minIndices[0]=j;
        }
        else if ( (m_vertices[j].y < mouse_world.y) && (mouse_world.y < m_vertices[j+m_lineLength].y) )
        {
            if ( m_vertices[j+m_lineLength].x - mouse_world.y  <  mouse_world.y - m_vertices[j].y )
                m_minIndices[0]=j+m_lineLength;
            else if (m_vertices[j+m_lineLength].y - mouse_world.y >  mouse_world.y - m_vertices[j].y )
                m_minIndices[0]=j;
        }
    }
}
