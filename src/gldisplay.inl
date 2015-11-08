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
        const qglviewer::Vec* va = data.getVertexArray();
        if(va)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, sizeof(qglviewer::Vec), va);
            glDrawArrays(glPrimitive(data.getPrimitive()), 0, data.getArrayLength());
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        else
            emit needsRebuild();
    }
        break;

    case megafi::MODE_VERTEX_INDICES:
    {
        const unsigned long* vi = data.getIndiceArray();
        if(vi)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, sizeof(qglviewer::Vec), data.getVertices());
            glDrawElements(glPrimitive(data.getPrimitive()),
                           data.getArrayLength(),
                           GL_UNSIGNED_INT,
                           vi);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
        break;
    }
}

#endif // GLDISPLAY_INL
