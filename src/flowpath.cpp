#include "flowpath.h"

#include <QDebug>

using namespace megafi;

FlowPath::FlowPath(const DTM& dtm, unsigned long origin) : m_dtm(dtm), m_minIndices()
{
    m_minIndices.push_back(origin);
    m_vertices.push_back(m_dtm.getVertices()[origin]);

    qDebug() << "First point's coordinates : x=" << m_vertices.back().x
             << ", y=" << m_vertices.back().y
             << ", z=" << m_vertices.back().z;

    computePath();
}

FlowPath::FlowPath(const FlowPath &other)
    : Drawable<FlowPath>(other),
      m_dtm(other.m_dtm), m_minIndices(other.m_minIndices)
{
}

FlowPath::~FlowPath()
{
}

void FlowPath::computePath()
{
    const qglviewer::Vec* const vertices   = m_dtm.getVertices();
    const unsigned long         vLength    = m_dtm.getNbVertices();
    const unsigned long         lineLength = m_dtm.getLineLength();

    do
    {
        unsigned long i = m_minIndices.last();

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
            qDebug() << "End flow path";
            break;
        }

        qDebug() << "Next point's coordinates : x=" << vertices[i].x
                 << ", y=" << vertices[i].y
                 << ", z=" << vertices[i].z;

        m_vertices.push_back(vertices[i]);
    } while(true);
}

inline void FlowPath::build_line_TRILINE(unsigned long i)
{
    buildFunction(i);
}

inline unsigned long FlowPath::array_size_TRILINE() const
{
    return m_vertices.size();
}

inline void FlowPath::buildInternal(unsigned int i)
{
    buildLine(i);
}
