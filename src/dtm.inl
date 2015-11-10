#ifndef DTM_INL
#define DTM_INL

#include "dtm.h"

namespace megafi
{

/*
 * GENERAL
 */

template<Primitive prim> inline void DTM::build_begin() const
{
    switch(getMode())
    {
    case MODE_LEGACY:
        glBegin(glPrimitive(getPrimitive()));
        break;
    case MODE_VERTEX_ARRAY:
        break;
    case MODE_VERTEX_INDICES:
        break;
    }
}

template<Primitive prim> inline void DTM::build_line(GLuint) {}
template<Primitive prim> inline void DTM::build_line(GLuint) const {}

template<Primitive prim> inline void DTM::build_back(GLuint) {}
template<Primitive prim> inline void DTM::build_back(GLuint) const {}

template<Primitive prim> inline void DTM::build_end() const
{
    switch(getMode())
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


#define TEMPLATE(fun, prim, arg, code) \
    template<> inline void DTM::fun<prim>(arg) { code } \
    template<> inline void DTM::fun<prim>(arg) const { code }

/*
 * POINTS
 */

TEMPLATE(build_line, POINT, GLuint i, buildFunction(i);)
#define ARRAY_SIZE_POINT NB_CALL_LINE



/*
 * EDGES
 */

TEMPLATE(build_line, TRILINE, GLuint i,
         buildFunction(i);
         if(i < m_vertices.size() - m_lineLength) // not at last line
             buildFunction(i + m_lineLength);)

TEMPLATE(build_back, TRILINE, GLuint j, buildFunction(j);)

#define ARRAY_SIZE_TRILINE NB_CALL_LINE*2 - m_lineLength + NB_CALL_BACK



TEMPLATE(build_line, QUADLINE, GLuint i,  build_line<TRILINE>(i);)

TEMPLATE(build_back, QUADLINE, GLuint j,
    if(j % m_lineLength) { // not at beginning of line
        buildFunction(j);
        buildFunction(j-1);
    })

#define ARRAY_SIZE_QUADLINE NB_CALL_LINE*2 - m_lineLength + 2*(NB_CALL_BACK - m_nbLines)



/*
 * SHAPES
 */

TEMPLATE(build_line, TRIFILL, GLuint i,
    buildFunction(i);
    if(i < m_vertices.size() - m_lineLength) // not at last line
        buildFunction(i + m_lineLength);
    else
        buildFunction(i);
)

#define ARRAY_SIZE_TRIFILL NB_CALL_LINE*2

TEMPLATE(build_line, QUADFILL, GLuint i, build_line<TRIFILL>(i);)

#define ARRAY_SIZE_QUADFILL ARRAY_SIZE_TRIFILL




/*
 * LINE LOOPS
 */

// Line loops are opened and closed at each time
template<> inline void DTM::build_begin<LINELOOP>() const {}
template<> inline void DTM::build_end  <LINELOOP>() const {}

TEMPLATE(build_line, LINELOOP, GLuint i,
    if(getMode() == MODE_LEGACY)
    {
        glBegin(GL_LINE_LOOP);

        buildFunction(i);

        if(i % m_lineLength != m_lineLength-1) { // not at end of line
            if(i < m_vertices.size())
                buildFunction(i+1);
        }

        if(i < m_vertices.size() - m_lineLength) // not at last line
            buildFunction(i + m_lineLength);

        glEnd();
    }
)

#define ARRAY_SIZE_LINELOOP getMode() == MODE_LEGACY ?\
                            NB_CALL_LINE*3 - m_nbLines - m_lineLength :\
                            0

/*
 * TRIANGLES
 */

TEMPLATE(build_line, TRIANGLES, GLuint i,
    if(i % m_lineLength && i < m_vertices.size() - m_lineLength) { // not at beginning nor last line
        buildFunction(i-1);
        buildFunction(i-1 + m_lineLength);
        buildFunction(i);

        buildFunction(i);
        buildFunction(i + m_lineLength);
        buildFunction(i-1 + m_lineLength);
    }
)

#define ARRAY_SIZE_TRIANGLES 6*(NB_CALL_LINE - m_lineLength - (m_nbLines-1))

}

#endif
