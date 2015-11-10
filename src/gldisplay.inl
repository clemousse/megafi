#ifndef GLDISPLAY_INL
#define GLDISPLAY_INL

#include "gldisplay.h"

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
        if(va)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, 0, va);
            glDrawArrays(glPrimitive(data.getPrimitive()), 0, data.getArrayLength());
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        else
            emit needsRebuild();
    }
        break;

    case megafi::MODE_VERTEX_INDICES:
    {
        const GLuint* vi = data.getIndiceArray();
        if(vi)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, 0, data.getVertices());
            glDrawElements(glPrimitive(data.getPrimitive()),
                           data.getArrayLength(),
                           GL_UNSIGNED_INT,
                           vi);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        else
            emit needsRebuild();
    }
        break;
    }

    {
        GLenum error = GL_NO_ERROR;
        while((error = glGetError()))
        {
            switch(error)
            {
            case GL_INVALID_ENUM                 : qWarning() << "GL returned error GL_INVALID_ENUM                 "; break;
            case GL_INVALID_VALUE                : qWarning() << "GL returned error GL_INVALID_VALUE                "; break;
            case GL_INVALID_OPERATION            : qWarning() << "GL returned error GL_INVALID_OPERATION            "; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: qWarning() << "GL returned error GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY                : qWarning() << "GL returned error GL_OUT_OF_MEMORY                "; break;
            case GL_STACK_UNDERFLOW              : qWarning() << "GL returned error GL_STACK_UNDERFLOW              "; break;
            case GL_STACK_OVERFLOW               : qWarning() << "GL returned error GL_STACK_OVERFLOW               "; break;
            }
        }
    }
}

#endif // GLDISPLAY_INL
