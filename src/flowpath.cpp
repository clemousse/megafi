#include "flowpath.h"

#include <QDebug>

using namespace megafi;


FlowPath::FlowPath(const DTM& dtm, unsigned long origin, const FlowPathProps* defaultProps, Mode mode)
    : Drawable(mode, TRILINE),
      m_minIndices(),
      m_defaultProps(defaultProps),
      m_props(m_defaultProps)
#if FALSE
      , endFP("Here the flow path")
#endif
{
    m_minIndices.push_back(origin);
}

FlowPath::FlowPath(const FlowPath &other)
    : Drawable(other),
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

void FlowPath::computePath(const DTM& dtm)
{
    const Point*  const vertices   = dtm.getVertices();
    const unsigned long vLength    = dtm.getNbVertices();
    const unsigned long lineLength = dtm.getLineLength();
    const Primitive     prim       = dtm.getPrimitive();

    qDebug() << "First point's coordinates : x="
             << vertices[m_minIndices.first()].x
             << ", y=" << vertices[m_minIndices.first()].y
             << ", z=" << vertices[m_minIndices.first()].z;

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
#if FALSE
            endFP = "End flow path";
#endif
            qDebug() << "End flow path";
            break;
        }

        else
        {
            Point p = vertices[argMin];
            p.z += 1.;
            m_minIndices.push_back(argMin);
            m_vertices.push_back(p);

            qDebug() << "Next point's coordinates"
                     << ": x=" << m_vertices.back().x
                     << ", y=" << m_vertices.back().y
                     << ", z=" << m_vertices.back().z;
        }
    }
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
