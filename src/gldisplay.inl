#ifndef GLDISPLAY_INL
#define GLDISPLAY_INL

#include "gldisplay.h"

namespace megafi
{

template<class Datatype>
void glDisplay::drawData(const Datatype& data) const
{
    switch(data.getMode())
    {
    case megafi::MODE_LEGACY:
        data.buildLegacy();
        break;

    case megafi::MODE_VERTEX_ARRAY:
    {
        const megafi::Point* va = data.getVertexArray();
        const megafi::Color* ca = data.getColorArray();
        if(va)
        {
            glVertexPointer(3, GL_DOUBLE, 0, va);
            if(ca) glColorPointer(3, GL_UNSIGNED_BYTE, 0, ca);

            glDrawArrays(glPrimitive(data.getPrimitive()), 0, data.getArrayLength());
        }
    }
        break;

    case megafi::MODE_VERTEX_INDICES:
    {
        const GLuint*        const vi = data.getIndiceArray();
        const megafi::Color* const ca = data.getColorArray();
        if(vi)
        {
            glVertexPointer(3, GL_DOUBLE, 0, data.getVertices());
            if(ca) glColorPointer(3, GL_UNSIGNED_BYTE, 0, ca);

            glDrawElements(glPrimitive(data.getPrimitive()),
                           data.getArrayLength(),
                           GL_UNSIGNED_INT,
                           vi);
        }
    }
        break;
    }

    {
        GLenum error = GL_NO_ERROR;
        while((error = glGetError()))
        {
            switch(error)
            {
            case GL_INVALID_ENUM                 : qWarning("GL returned error GL_INVALID_ENUM                 "); break;
            case GL_INVALID_VALUE                : qWarning("GL returned error GL_INVALID_VALUE                "); break;
            case GL_INVALID_OPERATION            : qWarning("GL returned error GL_INVALID_OPERATION            "); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: qWarning("GL returned error GL_INVALID_FRAMEBUFFER_OPERATION"); break;
            case GL_OUT_OF_MEMORY                : qWarning("GL returned error GL_OUT_OF_MEMORY                "); break;
            case GL_STACK_UNDERFLOW              : qWarning("GL returned error GL_STACK_UNDERFLOW              "); break;
            case GL_STACK_OVERFLOW               : qWarning("GL returned error GL_STACK_OVERFLOW               "); break;
            }
        }
    }
}

} // namespace megafi

#endif // GLDISPLAY_INL
