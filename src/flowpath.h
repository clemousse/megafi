#ifndef FLOWPATH_H
#define FLOWPATH_H

#include "drawable.h"
#include "dtm.h"

#include <QListWidgetItem>

namespace megafi
{

struct FlowPathProps
{
    float lineWidth;
    Color color;
};

class FlowPath : public Drawable, public QListWidgetItem
{
    Q_OBJECT

    static unsigned int m_number;

    QList<unsigned long> m_minIndices;
    const FlowPathProps* const m_defaultProps;
    const FlowPathProps* m_props;


public:
    FlowPath(const FlowPathProps* defaultProps, QListWidget *parent = NULL, Mode mode = MODE);
    FlowPath(const FlowPath& other);
    ~FlowPath();

    float getLineWidth() const;
    void  setProperties(const FlowPathProps* newProps);  

public slots:
    void buildArrays();
    void buildLegacy() const;
    void computePath(const megafi::DTM* dtm, unsigned long startIndex);

    // For GUI side
    void setName(const QString& newName);
    void buildIcon();

private:
    inline virtual void buildFunction(GLuint i) throw(const IncoherentMode&) override
    {
        buildColor(m_props->color);
        Drawable::buildFunction(i);
    }
    inline virtual void buildFunction(GLuint i) const throw(const IncoherentMode&) override
    {
        buildColor(m_props->color);
        Drawable::buildFunction(i);
    }
};

}

#endif // FLOWPATH_H
