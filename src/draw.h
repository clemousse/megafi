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

template <int primit> inline void glDisplay::draw_back(unsigned int) {}

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


/*
 * EDGES
 */

template<> inline void glDisplay::draw_line<DESIGN_EDGE>(unsigned int i) {
    draw_function(i);
    if(i < m_vertices.length() - m_lineLength) // not at last line
        draw_function(i + m_lineLength);
}

template<> inline void glDisplay::draw_back<DESIGN_EDGE | PRIM_TRIANGLES>(unsigned int j) {
    draw_function(j);
}

template<> inline void glDisplay::draw_back<DESIGN_EDGE | PRIM_SQUARES>(unsigned int j) {
    if(j % m_lineLength) { // not at beginning of line
        draw_function(j);
        draw_function(j-1);
    }
}




/*
 * SHAPES
 */

template<> inline void glDisplay::draw_line<DESIGN_SHAPE>(unsigned int i) {
    draw_function(i);
    if(i < m_vertices.length() - m_lineLength) // not at last line
        draw_function(i + m_lineLength);
    else
        draw_function(i);
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

    if(i % m_lineLength != m_lineLength-1) { // not at end of line
        if(i < static_cast<unsigned int>(m_vertices.length()))
            draw_function(i+1);
    }

    if(i < m_vertices.length() - m_lineLength) // not at last line
        draw_function(i + m_lineLength);

    glEnd();
}




/*
 * TRIANGLES
 */

template<> inline void glDisplay::draw_line<PRIM__TRIANGLES>(unsigned int i) {
    if(i % m_lineLength && i < m_vertices.length() - m_lineLength) { // not at beginning nor last line
        draw_function(i-1);
        draw_function(i-1 + m_lineLength);
        draw_function(i);

        draw_function(i);
        draw_function(i + m_lineLength);
        draw_function(i-1 + m_lineLength);
    }
}





#endif // DRAW_H
