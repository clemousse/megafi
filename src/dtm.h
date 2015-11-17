#ifndef DTM_H
#define DTM_H

#include "drawable.h"
#include "q_debugstream.h"

#include <QString>
#include <QProgressBar>

namespace megafi
{


class DTM : public Drawable
{
    Q_OBJECT

    // Members
private:
    // Data bounding rectangle
    qglviewer::Vec m_dataSizeMin;
    qglviewer::Vec m_dataSizeMax;

    double m_colorInterv;

    unsigned long m_lineLength;
    unsigned long m_nbLines;
    QProgressBar* m_progressBar;

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
    void progressB();

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
