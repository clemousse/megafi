#include "flowpath.h"

#include <QDebug>
#include <QSize>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <fstream>
#include <QFileDialog>

using namespace megafi;

unsigned int FlowPath::m_number = 0;

FlowPath::FlowPath(const FlowPathProps* defaultProps, QListWidget *parent, Mode mode)
    : Drawable(mode, TRILINE),
      QListWidgetItem(parent),
      m_minIndices(),
      m_defaultProps(defaultProps),
      m_props(m_defaultProps)
{
    ++m_number;
    setFlags(flags() | Qt::ItemIsEditable);
    setName(QString("Path #%1").arg(m_number));
    buildIcon();
}

FlowPath::FlowPath(const FlowPath &other)
    : Drawable(other), QListWidgetItem(other),
      m_minIndices(other.m_minIndices),
      m_defaultProps(other.m_defaultProps)
{
    if(other.m_props != m_defaultProps)
        m_props = new FlowPathProps(*other.m_props);
}

FlowPath::~FlowPath()
{
    if(m_props != m_defaultProps)
        delete m_props;
}

float FlowPath::getLineWidth() const { return m_props->lineWidth; }

void  FlowPath::setProperties(const FlowPathProps* newProps)
{
    if(newProps)
    {
        if(m_props != m_defaultProps)
            delete m_props;
        m_props = newProps;
    }
}

void FlowPath::computePath(const DTM *dtm, unsigned long startIndex)
{
    int countPoints_FP = 0;

    m_minIndices.push_back(startIndex);

    dtm->lock.lockForRead();

    const Point*  const vertices   = dtm->getVertices();
    const unsigned long vLength    = dtm->getNbVertices();
    const unsigned long lineLength = dtm->getLineLength();
    const Primitive     prim       = dtm->getPrimitive();

    qDebug() << "First point's coordinates :"
             << "\nx = " << vertices[m_minIndices.first()].x
             << "\ny = " << vertices[m_minIndices.first()].y
             << "\nz = " << vertices[m_minIndices.first()].z << "\n";

    m_vertices.push_back(vertices[m_minIndices.first()]);

    while(true)
    {
        const unsigned long i = m_minIndices.last();
        double zMin = vertices[i].z;
        unsigned long argMin = i;

        if( i%lineLength != 0 && vertices[i-1].z < zMin )
        {
            argMin = i-1;
            zMin   = vertices[i-1].z;
        }
        if( i%lineLength != lineLength-1 && vertices[i+1].z < zMin )
        {
            argMin = i+1;
            zMin   = vertices[i+1].z;
        }
        if( i >= lineLength && vertices[i-lineLength].z < zMin )
        {
            argMin = i - lineLength;
            zMin   = vertices[i-lineLength].z;
        }
        if( i < vLength-lineLength && vertices[i+lineLength].z < zMin)
        {
            argMin = i + lineLength;
            zMin   = vertices[i+lineLength].z;
        }
        if( prim & PRIM_TRIANGLES )
        {
            if( i%lineLength != 0 && i < vLength-lineLength
                 && vertices[i -1 +lineLength].z < zMin )
            {
                argMin = i -1 +lineLength;
                zMin   = vertices[i -1 +lineLength].z;
            }
            if( i%lineLength != lineLength-1 && i >= lineLength
                 && vertices[i +1 -lineLength].z < zMin )
            {
                argMin = i +1 -lineLength;
                zMin   = vertices[i +1 -lineLength].z;
            }
        }

        if(argMin == i)
        {

            qDebug() << "End flow path.\n";

            if (countPoints_FP<2)
            {
                 qWarning()<< "Flow path. It has"<< countPoints_FP <<"section.\n";
            }
            else
            {
                qWarning()<< "Flow path. It has"<< countPoints_FP <<"sections.\n";
            }

            break;
        }
        else
        {
            Point p = vertices[argMin];
            p.z += 1.;
            m_minIndices.push_back(argMin);
            m_vertices.push_back(p);


            qDebug() << "Next point's coordinates :"
                     << "\nx = " << m_vertices.back().x
                     << "\ny = " << m_vertices.back().y
                     << "\nz = " << m_vertices.back().z << "\n";

            countPoints_FP++;
        }
    }

    dtm->lock.unlock();
}



#define BUILD for(unsigned long i = 0; \
                  i < static_cast<unsigned long>(m_minIndices.size()) ;\
                  ++i) \
              { \
                  buildFunction(i); \
              }

void FlowPath::buildArrays()
{
    {
        const unsigned long size =
                static_cast<unsigned long>(m_minIndices.size());
        prepareBuild(size);
    }

    BUILD

    emit arrayRebuilt();
}

void FlowPath::buildLegacy() const
{
    if(getMode() == MODE_LEGACY)
    {
        glBegin(GL_LINE_STRIP);
        BUILD
        glEnd();
    }
}

void FlowPath::setName(const QString &newName)
{
    setText(newName);
}

void FlowPath::buildIcon()
{
    if(m_props)
    {
        // Standard dimension
        const QSize drawingSize(32, 32), margins(1,1);

        // Bounds
        const QPoint left(0 + margins.width(), drawingSize.height()/2 - m_props->lineWidth/2);
        const QSize  extend(drawingSize.width() - margins.width(), m_props->lineWidth);

        // Define the rectangle to be drawn
        const QRect  line(left, extend);

        // Color
        QColor qcolor;
        {
            Color color(m_props->color);
            qcolor.setRed(color.r);
            qcolor.setGreen(color.g);
            qcolor.setBlue(color.b);
            qcolor.setAlpha(255);
        }

        // Surface to be drawn
        QPixmap drawing(drawingSize);

        // Background
        {
            QColor background(0, 0, 0, 0);
            drawing.fill(background);
        }

        // Drawing
        {
            // Drawer
            QPainter painter(&drawing);
            painter.fillRect(line, qcolor);
        }

        // Transform it to an icon
        QIcon icon(drawing);

        setIcon(icon);
    }
}
