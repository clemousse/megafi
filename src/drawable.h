#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "draw.h"

#include <vector>

#include <QGLViewer/vec.h>

namespace megafi
{

template<class Child>
class Drawable
{
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

    // Setters
    void         changeMode(Mode m)           throw();
    void         changePrimitive(Primitive p) throw();

    // Building
    void         buildArrays(Child &ths) const;
    void         buildLegacy(const Child &ths) const;

private:
    // Functions to use in child class to draw elements
    void   (Child::*m_buildBegin)   ();
protected:
    void   (Child::*m_buildLine)    (unsigned long);
    void   (Child::*m_buildBack)    (unsigned long);
private:
    void   (Child::*m_buildEnd)     ();
protected:
    void   (Child::*m_buildFunction)(unsigned long);

#define buildLine(i)     (this->*m_buildLine)(i)
#define buildBack(j)     (this->*m_buildBack)(j)
#define buildFunction(i) (this->*m_buildFunction)(i)

private:
    // Default building functions
    inline void   build_begin();
    inline void   build_line(unsigned long);
    inline void   build_back(unsigned long);
    inline void   build_end();

protected:
    // Primitive building functions ; to be redefined
    void          build_begin_POINT    (     void      ) { build_begin(); }
    void          build_begin_TRILINE  (     void      ) { build_begin(); }
    void          build_begin_QUADLINE (     void      ) { build_begin(); }
    void          build_begin_TRIFILL  (     void      ) { build_begin(); }
    void          build_begin_QUADFILL (     void      ) { build_begin(); }
    void          build_begin_LINELOOP (     void      ) { build_begin(); }
    void          build_begin_TRIANGLES(     void      ) { build_begin(); }

    inline void   build_line_POINT     (unsigned long i) { build_line(i); }
    inline void   build_line_TRILINE   (unsigned long i) { build_line(i); }
    inline void   build_line_QUADLINE  (unsigned long i) { build_line(i); }
    inline void   build_line_TRIFILL   (unsigned long i) { build_line(i); }
    inline void   build_line_QUADFILL  (unsigned long i) { build_line(i); }
    inline void   build_line_LINELOOP  (unsigned long i) { build_line(i); }
    inline void   build_line_TRIANGLES (unsigned long i) { build_line(i); }

    inline void   build_back_POINT     (unsigned long j) { build_back(j); }
    inline void   build_back_TRILINE   (unsigned long j) { build_back(j); }
    inline void   build_back_QUADLINE  (unsigned long j) { build_back(j); }
    inline void   build_back_TRIFILL   (unsigned long j) { build_back(j); }
    inline void   build_back_QUADFILL  (unsigned long j) { build_back(j); }
    inline void   build_back_LINELOOP  (unsigned long j) { build_back(j); }
    inline void   build_back_TRIANGLES (unsigned long j) { build_back(j); }

    void          build_end_POINT      (     void      ) { build_end(); }
    void          build_end_TRILINE    (     void      ) { build_end(); }
    void          build_end_QUADLINE   (     void      ) { build_end(); }
    void          build_end_TRIFILL    (     void      ) { build_end(); }
    void          build_end_QUADFILL   (     void      ) { build_end(); }
    void          build_end_LINELOOP   (     void      ) { build_end(); }
    void          build_end_TRIANGLES  (     void      ) { build_end(); }

    inline unsigned long array_size_POINT     () const { return 0; }
    inline unsigned long array_size_TRILINE   () const { return 0; }
    inline unsigned long array_size_QUADLINE  () const { return 0; }
    inline unsigned long array_size_TRIFILL   () const { return 0; }
    inline unsigned long array_size_QUADFILL  () const { return 0; }
    inline unsigned long array_size_LINELOOP  () const { return 0; }
    inline unsigned long array_size_TRIANGLES () const { return 0; }

    void prepareBuild() throw(const std::bad_alloc&);
    virtual void buildInternal(unsigned int) {}
    void build(Child &ths) const;

private:
    void deleteArrays() throw();
    inline void build_function_legacy(unsigned long i);
    inline void build_function_va    (unsigned long i);
    inline void build_function_vi    (unsigned long i);
};

}

#include "drawable.cpp"

#endif // DRAWABLE_H
