#ifndef DRAW_H
#define DRAW_H

#include <stdexcept>
#include <string>

#include <GL/gl.h>

// The primitive that can be used
#define PRIM_TRIANGLES      0b00000001
#define PRIM_SQUARES        0b00000010

// The design that can be used
#define DESIGN_POINT        0b00000100
#define DESIGN_EDGE         0b00001000
#define DESIGN_SHAPE        0b00010000

// Other implemented primitives
#define PRIM__LINE_LOOP     0b00100000
#define PRIM__TRIANGLES     0b01000000


namespace megafi
{

enum Mode {
    MODE_LEGACY,
    MODE_VERTEX_ARRAY,
    MODE_VERTEX_INDICES
};

enum Primitive
{
    POINT    = DESIGN_POINT,
    TRILINE  = DESIGN_EDGE | PRIM_TRIANGLES,
    QUADLINE = DESIGN_EDGE | PRIM_SQUARES,
    TRIFILL  = DESIGN_SHAPE | PRIM_TRIANGLES,
    QUADFILL = DESIGN_SHAPE | PRIM_SQUARES,
    LINELOOP = PRIM__LINE_LOOP,
    TRIANGLES= PRIM__TRIANGLES
};

inline std::string mode2str(Mode m)
{
    switch(m)
    {
    case MODE_LEGACY: return "MODE_LEGACY"; break;
    case MODE_VERTEX_ARRAY: return "MODE_VERTEX_ARRAY"; break;
    case MODE_VERTEX_INDICES: return "MODE_VERTEX_INDICES"; break;
    default: return ""; break;
    }
}

class UnknownPrimitive : public std::logic_error
{
private:
    inline std::string composeMessage(const int prim, std::string userMessage) const
    {
        return std::string("Unknown primitive ") + std::to_string(prim) + (
                    !userMessage.empty()
                    ? std::string(" [") + userMessage + std::string("]")
                    : ""
                    );
    }

public:
    UnknownPrimitive() : logic_error("Unknown primitive") {}
    UnknownPrimitive(const int prim, std::string message = "")
        : std::logic_error(composeMessage(prim, message)) {}
};

inline int glPrimitive(Primitive p) throw()
{
    int ret = 0x000A; // does not exist

    switch(p)
    {
    case POINT:
        ret =  GL_POINTS; break;
    case TRILINE:
        ret = GL_LINE_STRIP; break;
    case QUADLINE:
        ret = GL_LINES; break;
    case TRIFILL:
        ret = GL_TRIANGLE_STRIP; break;
    case QUADFILL:
        ret = GL_QUAD_STRIP; break;
    case LINELOOP:
        ret = GL_LINE_LOOP; break;
    case TRIANGLES:
        ret = GL_TRIANGLES; break;
    default: // Should never happen !
        throw UnknownPrimitive(p);
    }

    return ret;
}

}

// FINAL CHOICE
#define MODE MODE_VERTEX_ARRAY
#define PRIM TRIFILL

#endif // DRAW_H
