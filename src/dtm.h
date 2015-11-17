#ifndef DTM_H
#define DTM_H

#include "drawable.h"

#include <QString>

namespace megafi
{



class DTM : public Drawable
{
    Q_OBJECT

    // Members
private:
    // data bounding rectangle
    qglviewer::Vec m_dataSizeMin;
    qglviewer::Vec m_dataSizeMax;

    double m_colorInterv;

    unsigned long m_lineLength;
    unsigned long m_nbLines;

public:
    DTM(Mode mode = MODE, Primitive prim = PRIM);
    DTM(const DTM& other);
    ~DTM();

    qglviewer::Vec getLL() const;
    qglviewer::Vec getUR() const;

    unsigned long getLineLength() const;
    unsigned long getNbLines   () const;

public slots:
    bool buildDTM(QString filePath);
    void buildArrays();
    void buildLegacy() const;
    unsigned long computeIndex(megafi::Point mouse_world) const;

signals:
    void indexComputed(unsigned long) const;

private:
    bool readDTM(const QString& path);
    void computeDataSize();
    void computeLineLength();

protected:
    Color computeColor(unsigned long index) const;

    template<Primitive prim> inline void build_begin() const;
    template<Primitive prim> inline void build_line(GLuint);
    template<Primitive prim> inline void build_line(GLuint) const;
    template<Primitive prim> inline void build_back(GLuint);
    template<Primitive prim> inline void build_back(GLuint) const;
    template<Primitive prim> inline void build_end() const;

    inline unsigned long array_size(Primitive prim) const;
    inline virtual void buildFunction(GLuint i) throw(const IncoherentMode&) override
    {
        buildColor(computeColor(i));
        Drawable::buildFunction(i);
    }
    inline virtual void buildFunction(GLuint i) const throw(const IncoherentMode&) override
    {
        buildColor(computeColor(i));
        Drawable::buildFunction(i);
    }
};


}
#endif // DTM_H
