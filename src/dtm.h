#ifndef DTM_H
#define DTM_H

#include "drawable.h"

#include <QString>

namespace megafi
{



class DTM : public Drawable<DTM>
{
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
    ~DTM();

    qglviewer::Vec getLL() const;
    qglviewer::Vec getUR() const;

    unsigned long getLineLength() const;
    unsigned long getNbLines   () const;

    unsigned long computeIndex(const qglviewer::Vec& mouse_world) const;

private:
    bool readDTM(const QString& path);
    void computeDataSize();
    void computeLineLength();

protected:
    void          build_begin_LINELOOP ();

    inline void   build_line_POINT     (unsigned long i);
    inline void   build_line_TRILINE   (unsigned long i);
    inline void   build_line_QUADLINE  (unsigned long i);
    inline void   build_line_TRIFILL   (unsigned long i);
    inline void   build_line_QUADFILL  (unsigned long i);
    inline void   build_line_LINELOOP  (unsigned long i);
    inline void   build_line_TRIANGLES (unsigned long i);

    inline void   build_back_TRILINE   (unsigned long j);
    inline void   build_back_QUADLINE  (unsigned long j);

    void          build_end_LINELOOP   ();

    inline unsigned long array_size_POINT     () const;
    inline unsigned long array_size_TRILINE   () const;
    inline unsigned long array_size_QUADLINE  () const;
    inline unsigned long array_size_TRIFILL   () const;
    inline unsigned long array_size_QUADFILL  () const;
    inline unsigned long array_size_LINELOOP  () const;
    inline unsigned long array_size_TRIANGLES () const;

public:
    void buildInternal(unsigned int i);
};


}
#endif // DTM_H
