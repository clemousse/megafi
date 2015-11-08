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

    unsigned long m_lineLength;
    unsigned long m_nbLines;

public:
    DTM();
    DTM(const QString& filePath, Mode mode = MODE, Primitive prim = PRIM);
    DTM(const DTM& other);
    ~DTM();

    qglviewer::Vec getLL() const;
    qglviewer::Vec getUR() const;

    unsigned long getLineLength() const;
    unsigned long getNbLines   () const;

    unsigned long computeIndex(const qglviewer::Vec& mouse_world) const;

public slots:
    void buildArrays();
    void buildLegacy() const;

private:
    bool readDTM(const QString& path);
    void computeDataSize();
    void computeLineLength();

protected:
    template<Primitive prim> inline void build_begin() const;
    template<Primitive prim> inline void build_line(unsigned long);
    template<Primitive prim> inline void build_line(unsigned long) const;
    template<Primitive prim> inline void build_back(unsigned long);
    template<Primitive prim> inline void build_back(unsigned long) const;
    template<Primitive prim> inline void build_end() const;

    inline unsigned long array_size(Primitive prim) const;
};


}
#endif // DTM_H
