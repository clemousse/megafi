#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "draw.h"

#include <vector>

#include <QObject>

#include <QGLViewer/vec.h>

namespace megafi
{

union Point
{
    struct
    {
        double x;
        double y;
        double z;
    };
    double v[3];
};

union Color
{
    struct
    {
        double r;
        double g;
        double b;
    };
    double v[3];
};

class Drawable : public QObject
{
    Q_OBJECT

protected:
    // Data
    std::vector<Point> m_vertices;

private:
    // Members
    Mode      m_mode;
    Primitive m_prim;

    unsigned long m_arrayLength;
    unsigned long m_arrayCurrent;
    // Vertex array
    Point*        m_vertexArray;
    // Indice array
    GLuint*       m_indicesArray;
    Color*        m_colorArray;

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
    virtual ~Drawable();

    // Getters
    Mode          getMode       () const throw();
    Primitive     getPrimitive  () const throw();

    const Point*  getVertices   () const throw();
    unsigned long getNbVertices () const throw();

    unsigned long getArrayLength() const throw();
    const Point*  getVertexArray() const throw();
    const GLuint* getIndiceArray() const throw();
    const Color*  getColorArray () const throw();

public slots:
    // Setters
    void          changeMode(Mode m)           throw();
    void          changePrimitive(Primitive p) throw();

    // Building
    virtual void buildArrays() =0;
    virtual void buildLegacy() const =0;

protected:
    void prepareBuild(unsigned long arrayLength) throw(const std::bad_alloc&);

    inline void buildFunction(GLuint i, double r, double g, double b) throw(const IncoherentMode&)
    {
        switch(m_mode)
        {
        case MODE_LEGACY:
            throw IncoherentMode("buildFunction", MODE_VERTEX_ARRAY, MODE_LEGACY);
        case MODE_VERTEX_ARRAY:
            build_function_va(i, r, g, b); break;
        case MODE_VERTEX_INDICES:
            build_function_vi(i, r, g, b); break;
        }
    }

    inline void buildFunction(GLuint i, double r, double g, double b) const throw(const IncoherentMode&)
    {
        switch(m_mode)
        {
        case MODE_LEGACY:
            build_function_legacy(i, r, g, b); break;
        case MODE_VERTEX_ARRAY:
            throw IncoherentMode("buildFunction", MODE_LEGACY, MODE_VERTEX_ARRAY);
        case MODE_VERTEX_INDICES:
            throw IncoherentMode("buildFunction", MODE_LEGACY, MODE_VERTEX_INDICES);
        }
    }

private:
    void deleteArrays() throw();
    inline void build_function_legacy(GLuint i, double r, double g, double b) const
    {
        glColor3d(r, g, b);
        glVertex3d(m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
    }

    inline void build_function_va    (GLuint i, double r, double g, double b)
    {
        m_vertexArray[m_arrayCurrent] = m_vertices[i];
        ++m_arrayCurrent;
    }

    inline void build_function_vi    (GLuint i, double r, double g, double b)
    {
        m_indicesArray[m_arrayCurrent] = i;
        ++m_arrayCurrent;
    }
};

}

#endif // DRAWABLE_H
