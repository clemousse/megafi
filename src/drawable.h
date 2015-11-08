#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "draw.h"

#include <vector>

#include <QObject>

#include <QGLViewer/vec.h>

namespace megafi
{


class Drawable
{
    Q_OBJECT

protected:
    // Data
    std::vector<qglviewer::Vec> m_vertices;

private:
    // Members
    Mode      m_mode;
    Primitive m_prim;

    unsigned long m_arrayLength;
    unsigned long m_arrayCurrent;
    // Vertex array
    qglviewer::Vec *m_vertexArray;
    // Indice array
    unsigned long  *m_indicesArray;

protected:
    class IncoherentMode : public std::logic_error
    {
    public:
        IncoherentMode(std::string function, Mode one, Mode two)
            : std::logic_error(
                  std::string("Attempted to call function ")
                  + function
                  + std::string(" recquiring mode ")
                  + std::string(mode2str(one))
                  + std::string(" with mode ")
                  + std::string(mode2str(two))
                  ) {}
        IncoherentMode(const IncoherentMode& other)
            : std::logic_error(other.what()) {}
    };

public:
    // Constructors
    Drawable(Mode mode = MODE, Primitive prim = PRIM) throw();
    Drawable(const Drawable& other) throw(const std::bad_alloc&);
    virtual ~Drawable() throw();

    // Getters
    Mode                  getMode       () const throw();
    Primitive             getPrimitive  () const throw();

    const qglviewer::Vec* getVertices   () const throw();
    unsigned long         getNbVertices () const throw();

    unsigned long         getArrayLength() const throw();
    const qglviewer::Vec* getVertexArray() const throw();
    const unsigned long * getIndiceArray() const throw();

public slots:
    // Setters
    void         changeMode(Mode m)           throw();
    void         changePrimitive(Primitive p) throw();

    // Building
    virtual void buildArrays();
    virtual void buildLegacy() const;

protected:
    inline void buildFunction(unsigned long i) throw(const IncoherentMode&);
    inline void buildFunction(unsigned long i) const throw(const IncoherentMode&);
    void prepareBuild(unsigned long arrayLength) throw(const std::bad_alloc&);

private:
    void deleteArrays() throw();
    inline void build_function_legacy(unsigned long i) const;
    inline void build_function_va    (unsigned long i);
    inline void build_function_vi    (unsigned long i);
};

}

#endif // DRAWABLE_H
