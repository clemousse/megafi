#ifndef DRAWABLE_CPP
#define DRAWABLE_CPP

#include "drawable.h"

using namespace megafi;
using namespace qglviewer;

template<class Child>
Drawable<Child>::Drawable(Mode mode, Primitive prim) throw()
    : m_vertices(),
      m_mode(mode), m_prim(prim),
      m_arrayLength(0), m_arrayCurrent(0),
      m_vertexArray(NULL),
      m_indicesArray(NULL),
      m_buildBegin(NULL),
      m_buildLine(NULL),
      m_buildBack(NULL),
      m_buildEnd(NULL),
      m_buildFunction(NULL)
{
}

template<class Child>
Drawable<Child>::Drawable(const Drawable& other) throw(const std::bad_alloc&)
    : Drawable(other.m_mode, other.m_prim)
{
    const unsigned long nbVert = other.m_vertices.size();
    if(nbVert)
    {
        m_vertices.reserve(nbVert);
        for(unsigned long i = 0; i < nbVert; ++i)
        {
            m_vertices[i] = other.m_vertices[i];
        }
    }

    try
    {
        m_arrayLength = other.m_arrayLength;
        if(other.m_vertexArray)
        {
            m_vertexArray = new qglviewer::Vec[m_arrayLength];
            for(unsigned long i = 0; i < m_arrayLength; ++i)
            {
                m_vertexArray[i] = other.m_vertexArray[i];
            }
        }

        try
        {
            if(other.m_indicesArray)
            {
                m_indicesArray = new unsigned long[m_arrayLength];
                for(unsigned long i = 0; i < m_arrayLength; ++i)
                {
                    m_indicesArray[i] = other.m_indicesArray[i];
                }
            }
        }
        catch(const std::bad_alloc&)
        {
            m_indicesArray = NULL;
            delete[] m_vertexArray;
            throw;
        }
        catch(...)
        {
            delete[] m_indicesArray;
            m_indicesArray = NULL;
            throw;
        }
    }
    catch(const std::bad_alloc&)
    {
        m_vertexArray = NULL;
        m_arrayLength = 0;
        m_vertices.clear();
        throw;
    }
    catch(...)
    {
        delete[] m_vertexArray;
        m_vertexArray = NULL;
        m_arrayLength = 0;
        m_vertices.clear();
        throw;
    }
}

template<class Child>
Drawable<Child>::~Drawable() throw()
{
    if(m_arrayLength)
    {
        if(m_vertexArray)  delete[] m_vertexArray;
        if(m_indicesArray) delete[] m_indicesArray;
    }
}

template<class Child>
void Drawable<Child>::deleteArrays() throw()
{
    if(m_arrayLength)
    {
        switch(m_mode)
        {
        case MODE_LEGACY: break;
        case MODE_VERTEX_ARRAY:
            delete[] m_vertexArray;
            m_vertexArray = NULL;
            break;
        case MODE_VERTEX_INDICES:
            delete[] m_indicesArray;
            m_indicesArray = NULL;
            break;
        }
        m_arrayLength = 0;
    }
}

template<class Child>
Mode Drawable<Child>::getMode() const throw()
{
    return m_mode;
}

template<class Child>
Primitive Drawable<Child>::getPrimitive() const throw()
{
    return m_prim;
}

template<class Child>
const qglviewer::Vec* Drawable<Child>::getVertices() const throw()
{
    return m_vertices.data();
}

template<class Child>
unsigned long Drawable<Child>::getNbVertices() const throw()
{
    return m_vertices.size();
}

template<class Child>
unsigned long Drawable<Child>::getArrayLength() const throw()
{
    return m_arrayLength;
}

template<class Child>
const Vec* Drawable<Child>::getVertexArray() const throw()
{
    return m_vertexArray;
}

template<class Child>
const unsigned long* Drawable<Child>::getIndiceArray() const throw()
{
    return m_indicesArray;
}

template<class Child>
void Drawable<Child>::changeMode(Mode m) throw()
{
    deleteArrays();
    m_mode = m;
    m_buildFunction = NULL;
    buildArrays();
}

template<class Child>
void Drawable<Child>::changePrimitive(Primitive p) throw()
{
    deleteArrays();
    m_prim = p;
    m_buildBegin = NULL;
    m_buildLine  = NULL;
    m_buildBack  = NULL;
    m_buildEnd   = NULL;
    buildArrays();
}

template<class Child>
inline void Drawable<Child>::build_function_legacy(unsigned long i)
{
    glVertex3d(m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
}

template<class Child>
inline void Drawable<Child>::build_function_va(unsigned long i)
{
    m_vertexArray[m_arrayCurrent] = m_vertices[i];
    ++m_arrayCurrent;
}

template<class Child>
inline void Drawable<Child>::build_function_vi(unsigned long i)
{
    m_indicesArray[m_arrayCurrent] = i;
    ++m_arrayCurrent;
}

template<class Child>
void Drawable<Child>::prepareBuild() throw(const std::bad_alloc&)
{
    deleteArrays();

    switch(m_prim)
    {
    case POINT:
        m_buildBegin = &Child::build_begin_POINT;
        m_buildLine  = &Child:: build_line_POINT;
        m_buildBack  = &Child:: build_back_POINT;
        m_buildEnd   = &Child::  build_end_POINT;
        m_arrayLength= &Child:: array_size_POINT;
        break;
    case TRILINE:
        m_buildBegin = &Child::build_begin_TRILINE;
        m_buildLine  = &Child:: build_line_TRILINE;
        m_buildBack  = &Child:: build_back_TRILINE;
        m_buildEnd   = &Child::  build_end_TRILINE;
        m_arrayLength= &Child:: array_size_TRILINE;
        break;
    case QUADLINE:
        m_buildBegin = &Child::build_begin_QUADLINE;
        m_buildLine  = &Child:: build_line_QUADLINE;
        m_buildBack  = &Child:: build_back_QUADLINE;
        m_buildEnd   = &Child::  build_end_QUADLINE;
        m_arrayLength= &Child:: array_size_QUADLINE;
        break;
    case TRIFILL:
        m_buildBegin = &Child::build_begin_TRIFILL;
        m_buildLine  = &Child:: build_line_TRIFILL;
        m_buildBack  = &Child:: build_back_TRIFILL;
        m_buildEnd   = &Child::  build_end_TRIFILL;
        m_arrayLength= &Child:: array_size_TRIFILL;
        break;
    case QUADFILL:
        m_buildBegin = &Child::build_begin_QUADFILL;
        m_buildLine  = &Child:: build_line_QUADFILL;
        m_buildBack  = &Child:: build_back_QUADFILL;
        m_buildEnd   = &Child::  build_end_QUADFILL;
        m_arrayLength= &Child:: array_size_QUADFILL;
        break;
    case LINELOOP:
        m_buildBegin = &Child::build_begin_LINELOOP;
        m_buildLine  = &Child:: build_line_LINELOOP;
        m_buildBack  = &Child:: build_back_LINELOOP;
        m_buildEnd   = &Child::  build_end_LINELOOP;
        m_arrayLength= &Child:: array_size_LINELOOP;
        break;
    case TRIANGLES:
        m_buildBegin = &Child::build_begin_TRIANGLES;
        m_buildLine  = &Child:: build_line_TRIANGLES;
        m_buildBack  = &Child:: build_back_TRIANGLES;
        m_buildEnd   = &Child::  build_end_TRIANGLES;
        m_arrayLength= &Child:: array_size_TRIANGLES;
        break;
    default:
        throw UnknownPrimitive(m_prim);
    }

    switch(m_mode)
    {
    case MODE_LEGACY:
        m_buildFunction = &Child::build_function_legacy;
        break;

    case MODE_VERTEX_ARRAY:
        if(m_arrayLength)
        {
            try
            {
                m_vertexArray = new Vec[m_arrayLength];
            }
            catch(const std::bad_alloc&)
            {
                m_arrayLength = 0;
                m_vertexArray = NULL;
                throw;
            }
        }
        m_buildFunction = &build_function_va;
        m_arrayCurrent = 0;
        break;

    case MODE_VERTEX_INDICES:
        if(m_arrayLength)
        {
            try
            {
                m_indicesArray = new unsigned long[m_arrayLength];
            }
            catch(const std::bad_alloc&)
            {
                m_arrayLength = 0;
                m_indicesArray = NULL;
                throw;
            }
        }
        m_buildFunction = &build_function_vi;
        m_arrayCurrent = 0;
        break;
    }
}


template<class Child>
void Drawable<Child>::buildArrays(Child& ths) const
{
    if(m_mode != MODE_LEGACY)
        build(ths);
}

template<class Child>
void Drawable<Child>::buildLegacy(const Child &ths) const
{
    if(m_mode == MODE_LEGACY)
        build(ths);
}

template<class Child>
void Drawable<Child>::build(Child &ths) const
{
    const unsigned long vLength = m_vertices.size();

    (ths.*m_buildBegin)();

    for(unsigned long i = 0 ; i < vLength ; ++i)
    {
        ths.buildInternal(i);
    }

    (ths.*m_buildEnd)();
}


/*
 * GENERAL
 */

template<class Child>
inline void Drawable<Child>::build_begin()
{
    switch(m_mode)
    {
    case MODE_LEGACY:
        glBegin(glPrimitive(m_prim));
        break;
    case MODE_VERTEX_ARRAY:
        break;
    case MODE_VERTEX_INDICES:
        break;
    }
}

template<class Child>
inline void Drawable<Child>::build_line(unsigned long) {}

template<class Child>
inline void Drawable<Child>::build_back(unsigned long) {}

template<class Child>
inline void Drawable<Child>::build_end()
{
    switch(m_mode)
    {
    case MODE_LEGACY:
        glEnd();
        break;
    case MODE_VERTEX_ARRAY:
        break;
    case MODE_VERTEX_INDICES:
        break;
    }
}

#endif // DRAWABLE_CPP
