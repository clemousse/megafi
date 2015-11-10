#ifndef FLOWPATH_H
#define FLOWPATH_H

#include "drawable.h"
#include "dtm.h"
#include "flowpathview.h"

namespace megafi
{

class FlowPath : public Drawable
{
    QList<unsigned long> m_minIndices;
    const FlowPathProps* const m_defaultProps;
    const FlowPathProps* m_props;

#if FALSE
public:
    QString endFP;
#endif

public:
    FlowPath(const DTM& dtm, unsigned long origin, const FlowPathProps* defaultProps, Mode mode = MODE);
    FlowPath(const FlowPath& other);
    ~FlowPath();

public slots:
    void buildArrays();
    void buildLegacy() const;

protected:
    void computePath(const DTM& dtm);
};

}

#endif // FLOWPATH_H
