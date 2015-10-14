#include "gldisplay.h"

glDisplay::glDisplay(MainWindow * mainW) :
    QWindow(), // on appelle toujours le constructeur de la classe parente en premier
    QOpenGLFunctions(),
    m_mainW(mainW),
    m_context(new(std::nothrow) QOpenGLContext),
    m_windowSize(400, 300),
    m_initOK(false)
{
    setWidth(m_windowSize.width());
    setHeight(m_windowSize.height());

    // This window will be rendered with openGL
    setSurfaceType(QWindow::OpenGLSurface);

    // Creating the drawing context
    if(m_context)
    {
        if(!m_context->create())
            throw initException("Unable to create openGL context.");
    }
    else throw new std::bad_alloc;
}

glDisplay::~glDisplay()
{
    delete m_context;
}

void glDisplay::reshapeWindow(int width, int height)
{
    m_windowSize.setWidth(width);
    m_windowSize.setHeight(height);
}

void glDisplay::initialize()
{
    // If the window is not exposed, it will not be drawn,
    // so openGL will not know anything about the graphic processor ;
    // it cannot be initialized, thus.
    if(isExposed())
    {
        // Initialize openGL
        initializeOpenGLFunctions();
        // Background color
        glClearColor(255, 255, 255, 255);

        m_initOK = true;
    }
}

void glDisplay::render()
{
    // Ensure the right drawing context will be used
    m_context->makeCurrent(this);

    if(!m_initOK)
        initialize();

    if(m_initOK) // else, cannot draw anything
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear screen

        m_context->swapBuffers(this);
    }
}

bool glDisplay::event(QEvent * event)
{
    bool processed = false; // Has the event been successfully processed?
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        render();
        processed = true;
        break;
    default:
        // pass the event to the parent class to have the default behavior
        processed = QWindow::event(event);
    }
    return processed;
}

void glDisplay::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event); // prevent the event to be processed

    if (isExposed())
        render();
}

