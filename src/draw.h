#ifndef DRAW_H
#define DRAW_H

#include "gldisplay.h"

/*
 * GENERAL CASE
 */

template <int primit> inline void glDisplay::draw_init() {
#if MODE == MODE_LEGACY
    int glprim = 0;
    OPENGL(primit, glprim)
    glBegin(glprim);
#endif
}

template <int primit> inline void glDisplay::draw_line(unsigned int) {}

template <int primit> inline void glDisplay::draw_beginline(unsigned int) {}

template <int primit> inline void glDisplay::draw_end() {
#if MODE == MODE_LEGACY
    glEnd();
#endif
}





/*
 * POINTS
 */

template<> inline void glDisplay::draw_line<DESIGN_POINT>(unsigned int i) {
    draw_function(i);
}

template<> inline void glDisplay::draw_beginline<DESIGN_POINT>(unsigned int i) {
    draw_line<DESIGN_POINT>(i);
}






/*
 * TRIANGLE EDGES
 */

// Line strip needs a point before start
template<> inline void glDisplay::draw_init<DESIGN_EDGE | PRIM_TRIANGLES>() {
#if MODE == MODE_LEGACY
    glBegin(GL_LINE_STRIP);
#endif

    if(m_lineLength > 0)
        draw_function(m_lineLength -1);
}

template<> inline void glDisplay::draw_line<DESIGN_EDGE | PRIM_TRIANGLES>(unsigned int i) {
    draw_function(i);
    if(i < m_vertices.length() - m_lineLength) // not at last line
        draw_function(i + m_lineLength);
}

template<> inline void glDisplay::draw_beginline<DESIGN_EDGE | PRIM_TRIANGLES>(unsigned int i) {
    glDisplay::draw_line<DESIGN_EDGE | PRIM_TRIANGLES>(i);
}







/*
 * TRIANGLE SHAPES
 */

template<> inline void glDisplay::draw_line<DESIGN_SHAPE | PRIM_TRIANGLES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i); draw_function(i + m_lineLength);
    }
}

template<> inline void glDisplay::draw_beginline<DESIGN_SHAPE | PRIM_TRIANGLES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
#if MODE == LEGACY
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
#else
        if(i > 0) {
            draw_function(i-1);
            draw_function(i);
            draw_function(i);
        }
#endif
        draw_function(i);
        draw_function(i + m_lineLength);
    }
}







/*
 * SQUARE EDGES
 */

// Line strip needs a point before start
template<> inline void glDisplay::draw_init<DESIGN_EDGE | PRIM_SQUARES>() {
#if MODE == MODE_LEGACY
    glBegin(GL_LINE_STRIP);
#endif

    if(m_lineLength > 0)
        draw_function(m_lineLength -1);
}

template<> inline void glDisplay::draw_line<DESIGN_EDGE | PRIM_SQUARES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        const unsigned int
                first  = (i % m_lineLength) % 2 ? (i + m_lineLength) : i,
                second = (i % m_lineLength) % 2 ? i : (i + m_lineLength);
        draw_function(first);
        draw_function(second);
    }
}

template<> inline void glDisplay::draw_beginline<DESIGN_EDGE | PRIM_SQUARES>(unsigned int i) {
    draw_function(i-1 + m_lineLength);
    glDisplay::draw_line<DESIGN_EDGE | PRIM_SQUARES>(i);
}

template<> inline void glDisplay::draw_end<DESIGN_EDGE | PRIM_SQUARES>() {
    if(m_vertices.length() >= m_lineLength)
    {
        draw_function(m_vertices.length() -1);
        draw_function(m_vertices.length() - m_lineLength);
    }
#if MODE == MODE_LEGACY
        glEnd();
#endif
}






/*
 * SQUARE SHAPES
 */

template<> inline void glDisplay::draw_line<DESIGN_SHAPE | PRIM_SQUARES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i);
        draw_function(i + m_lineLength);
    }
}

template<> inline void glDisplay::draw_beginline<DESIGN_SHAPE | PRIM_SQUARES>(unsigned int i) {
#if MODE == MODE_LEGACY
    glEnd();
    glBegin(GL_QUAD_STRIP);
#else
    if(i > 0) {
        draw_function(i-1);
        draw_function(i-1);
        draw_function(i);
        draw_function(i);
    }
#endif
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i);
        draw_function(i + m_lineLength);
    }
}







/*
 * LINES
 */

template<> inline void glDisplay::draw_line<PRIM__LINES>(unsigned int i) {
    draw_function(i-1); draw_function(i);
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i); draw_function(i + m_lineLength);
    }
}

template<> inline void glDisplay::draw_beginline<PRIM__LINES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i); draw_function(i + m_lineLength);
    }
}





/*
 * LINE LOOPS
 */

// Line loops are opened and closed at each time
template<> inline void glDisplay::draw_init<PRIM__LINE_LOOP>() {}
template <> inline void glDisplay::draw_end<PRIM__LINE_LOOP>() {}

template<> inline void glDisplay::draw_line<PRIM__LINE_LOOP>(unsigned int i) {
    glBegin(GL_LINE_LOOP);
    draw_function(i);
    draw_function(i-1);
    if(i < m_vertices.length() - m_lineLength) // not at last line
        draw_function(i-1 + m_lineLength);
    glEnd();
}

template<> inline void glDisplay::draw_beginline<PRIM__LINE_LOOP>(unsigned int i) {
    if(i > 0) {
        glBegin(GL_LINES);
        draw_function(i-1);
        draw_function(i-1 + m_lineLength);
        glEnd();
    }
}




/*
 * TRIANGLES
 */

template<> inline void glDisplay::draw_line<PRIM__TRIANGLES>(unsigned int i) {
    if(i < m_vertices.length() - m_lineLength) { // not at last line
        draw_function(i-1);
        draw_function(i-1 + m_lineLength);
        draw_function(i);

        draw_function(i);
        draw_function(i + m_lineLength);
        draw_function(i-1 + m_lineLength);
    }
}





#endif // DRAW_H
