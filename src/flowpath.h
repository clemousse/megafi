#ifndef FLOWPATH_H
#define FLOWPATH_H

#include "drawable.h"
#include "dtm.h"

#include <QList>

namespace megafi
{

class FlowPath : public Drawable<FlowPath>
{
    const DTM& m_dtm;
    QList<unsigned long> m_minIndices;

public:
    FlowPath(const DTM& m_dtm, unsigned long origin);
    FlowPath(const FlowPath& other);
    ~FlowPath();

protected:
    void computePath();
    inline void build_line_TRILINE(unsigned long i);
    inline unsigned long array_size_TRILINE() const;
    inline void buildInternal(unsigned int i);
};

}

#endif // FLOWPATH_H
