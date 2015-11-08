#include "drawable.h"

using namespace megafi;
using namespace qglviewer;

Drawable::Drawable(Mode mode, Primitive prim) throw()
    : m_vertices(),
      m_mode(mode), m_prim(prim),
      m_arrayLength(0), m_arrayCurrent(0),
      m_vertexArray(NULL),
      m_indicesArray(NULL)
{
}


Drawable::Drawable(const Drawable& other) throw(const std::bad_alloc&)
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
            m_vertexArray = new Vec[m_arrayLength];
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


Drawable::~Drawable() throw()
{
    if(m_arrayLength)
    {
        if(m_vertexArray)  delete[] m_vertexArray;
        if(m_indicesArray) delete[] m_indicesArray;
    }
}


Mode Drawable::getMode() const throw()
{
    return m_mode;
}

Primitive Drawable::getPrimitive() const throw()
{
    return m_prim;
}

const Vec* Drawable::getVertices() const throw()
{
    return m_vertices.data();
}

unsigned long Drawable::getNbVertices() const throw()
{
    return m_vertices.size();
}

unsigned long Drawable::getArrayLength() const throw()
{
    return m_arrayLength;
}

const Vec* Drawable::getVertexArray() const throw()
{
    return m_vertexArray;
}

const unsigned long* Drawable::getIndiceArray() const throw()
{
    return m_indicesArray;
}

void Drawable::changeMode(Mode m) throw()
{
    deleteArrays();
    m_mode = m;
}

void Drawable::changePrimitive(Primitive p) throw()
{
    deleteArrays();
    m_prim = p;
}

void Drawable::deleteArrays() throw()
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


void Drawable::prepareBuild(unsigned long arrayLength)
throw(const std::bad_alloc&)
{
    deleteArrays();
    m_arrayLength = arrayLength;

    switch(m_mode)
    {
    case MODE_LEGACY:
        m_arrayLength = 0;
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
        m_arrayCurrent = 0;
        break;
    }
}
