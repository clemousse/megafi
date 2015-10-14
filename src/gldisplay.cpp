#include "gldisplay.h"

glDisplay::glDisplay(MainWindow * mainW) :
    QWindow(), // on appelle toujours le constructeur de la classe parente en premier
    m_mainW(mainW)
{
}

glDisplay::~glDisplay()
{
}
