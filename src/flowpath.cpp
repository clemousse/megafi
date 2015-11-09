#include "flowpath.h"

#include <QDebug>

using namespace megafi;


FlowPath::FlowPath(const DTM& dtm, unsigned long origin, Mode mode)
    : Drawable(mode, TRILINE), m_minIndices()
#if FALSE
      , endFP("Here the flow path")
#endif
{
    m_minIndices.push_back(origin);
    computePath(dtm);
}

FlowPath::FlowPath(const FlowPath &other)
    : Drawable(other), m_minIndices(other.m_minIndices)
{
}

FlowPath::~FlowPath()
{
}

void FlowPath::computePath(const DTM& dtm)
{
    const qglviewer::Vec* const vertices   = dtm.getVertices();
    const unsigned long         vLength    = dtm.getNbVertices();
    const unsigned long         lineLength = dtm.getLineLength();

    qDebug() << "First point's coordinates : x="
             << vertices[m_minIndices.first()].x
             << ", y=" << vertices[m_minIndices.first()].y
             << ", z=" << vertices[m_minIndices.first()].z;

    while(true)
    {
        const unsigned long i = m_minIndices.last();

        if( i%lineLength != 0
                && vertices[i-1].z < vertices[i].z )
            m_minIndices.push_back(i-1);

        else if( i%lineLength != lineLength-1
                 && vertices[i+1].z < vertices[i].z)
            m_minIndices.push_back(i+1);

        else if( i >= lineLength
                 && vertices[i-lineLength].z < vertices[i].z)
            m_minIndices.push_back(i-lineLength);

        else if( i < vLength-lineLength
                 && vertices[i+lineLength].z < vertices[i].z)
            m_minIndices.push_back(i+lineLength);
        else
        {
#if FALSE
            endFP = "End flow path";
#endif
            qDebug() << "End flow path";
            break;
        }

        qDebug() << "Next point's coordinates : x="
                 << vertices[m_minIndices.last()].x
                 << ", y=" << vertices[m_minIndices.last()].y
                 << ", z=" << vertices[m_minIndices.last()].z;
    }

    for(QList<unsigned long>::const_iterator index = m_minIndices.cbegin() ;
        index != m_minIndices.cend() ;
        ++index)
    {
        m_vertices.push_back(vertices[*index]);
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
