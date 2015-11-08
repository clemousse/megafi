#include "dtm.h"

#include <QFile>
#include <QDebug>

using namespace megafi;

DTM::DTM()
    : m_dataSizeMin(), m_dataSizeMax(),
      m_lineLength(0),
      m_nbLines(0)
{
}

DTM::DTM(const QString &filePath, Mode mode, Primitive prim)
    : Drawable(mode, prim),
      m_dataSizeMin(), m_dataSizeMax(),
      m_lineLength(0),
      m_nbLines(0)
{
    if(readDTM(filePath))
    {
        computeDataSize();
        computeLineLength();
        buildArrays();
    }
}

DTM::DTM(const DTM &other)
    : Drawable(other),
      m_dataSizeMin(other.m_dataSizeMin),
      m_dataSizeMax(other.m_dataSizeMax),
      m_lineLength(other.m_lineLength),
      m_nbLines(other.m_nbLines)
{
}

DTM::~DTM()
{
}

qglviewer::Vec DTM::getLL() const
{
    return m_dataSizeMin;
}

qglviewer::Vec DTM::getUR() const
{
    return m_dataSizeMax;
}

unsigned long DTM::getLineLength() const
{
    return m_lineLength;
}

unsigned long DTM::getNbLines() const
{
    return m_nbLines;
}

/* Purpose: read the coordinates from a DTM file and store them in a QVector
 * Arguments:
 *     filePath: path and filename of the file to be read
 * Return: true if the file have been opened
 */
bool DTM::readDTM(const QString& path)
{
    bool bOpen = false;

    // Opening the file
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bOpen = true;

        // Convert the file into a text stream (un flux de texte) for better readability
        QTextStream filestream(&file);

        // Declare variables to be used in the loop
        QString line;
        QStringList coordinates;
        qglviewer::Vec p;

        while(!filestream.atEnd())
        {
            // The file is read line-by-line
            line = filestream.readLine();
            // A line contains three numbers; make one string (character sequence) for each number
            coordinates = line.split(' ');

            // Convert each string to a real machine number
            double a = coordinates[0].toDouble();
            double b = coordinates[1].toDouble();
            double c = coordinates[2].toDouble();

            // Store the coordinates into a Point structure
            p.x=a;
            p.y=b;
            p.z=c;

            // Add the three coordinates at the end of the vector
            m_vertices.push_back(p);
        }

        // The file has been opened, close it now
        file.close();
    }

    return bOpen;
}

void DTM::computeDataSize()
{
    m_dataSizeMin.x = INFINITY;
    m_dataSizeMin.y = INFINITY;
    m_dataSizeMin.z = INFINITY;
    m_dataSizeMax.x = 0;
    m_dataSizeMax.y = 0;
    m_dataSizeMax.z = 0;


    for(unsigned long i = 0 ; i < m_vertices.size() ; ++i)
    {
        if(m_vertices[i].x < m_dataSizeMin.x) m_dataSizeMin.x = m_vertices[i].x;
        if(m_vertices[i].x > m_dataSizeMax.x) m_dataSizeMax.x = m_vertices[i].x;
        if(m_vertices[i].y < m_dataSizeMin.y) m_dataSizeMin.y = m_vertices[i].y;
        if(m_vertices[i].y > m_dataSizeMax.y) m_dataSizeMax.y = m_vertices[i].y;
        if(m_vertices[i].z < m_dataSizeMin.z) m_dataSizeMin.z = m_vertices[i].z;
        if(m_vertices[i].z > m_dataSizeMax.z) m_dataSizeMax.z = m_vertices[i].z;
    }

    qDebug() << "MNT is between (" << m_dataSizeMin.x << ',' << m_dataSizeMin.y << ',' << m_dataSizeMin.z
             << ") and (" << m_dataSizeMax.x << ',' << m_dataSizeMax.y << ',' << m_dataSizeMax.z << ")";
}

void DTM::computeLineLength()
{
    const long NOT_COMPUTED = 0;
    unsigned long lineLength_prec = NOT_COMPUTED;
    m_lineLength = 1;
    m_nbLines = 1;

    // at each loop, m_lineLenght takes 1 if "m_vertices[i].y == m_vertices[i+1].y".
    // m_lineLenght begin at 1.
    for(unsigned long i = 0 ; i < m_vertices.size()-1; ++i, ++m_lineLength)
    {
          // when "m_vertices[i].y != m_vertices[i+1].y", we compare m_lineLenght and lineLenght_prec
          // to see if they have the same number of points.
          if (m_vertices[i].y != m_vertices[i+1].y)
          {
             if(lineLength_prec != NOT_COMPUTED && m_lineLength != lineLength_prec)
                 qWarning() << "Lines" << i << "and" << i+1 << "don't have the same length.";

             lineLength_prec = m_lineLength;
             m_lineLength = 0;
             ++m_nbLines;
          }
    }

    m_lineLength = lineLength_prec;
    qDebug() << "Lines are" << m_lineLength << "points long.";
}

unsigned long DTM::computeIndex(const qglviewer::Vec& mouse_world) const
{
    unsigned long ix = 0;
    unsigned long index = 0;

    // Retrouver l'indice du point qui a pour coordonnées mouse_world
    for(unsigned long i = 0 ; i < m_lineLength-1; i+=1)
    {
        if (m_vertices[i].x == mouse_world.x)
        {
            ix = i;
        }
        else if ( (m_vertices[i].x < mouse_world.x) && (mouse_world.x < m_vertices[i+1].x) )
        {
            if(m_vertices[i+1].x - mouse_world.x < mouse_world.x - m_vertices[i].x)
                ix = i+1;
            else if (m_vertices[i+1].x - mouse_world.x > mouse_world.x - m_vertices[i].x)
                ix = i;
        }
    }

    for(unsigned long j = ix ; j < m_vertices.size()-m_lineLength; j+=m_lineLength)
    {
        if (m_vertices[j].y == mouse_world.y )
        {
            index=j;
        }
        else if ( (m_vertices[j].y < mouse_world.y) && (mouse_world.y < m_vertices[j+m_lineLength].y) )
        {
            if ( m_vertices[j+m_lineLength].x - mouse_world.y  <  mouse_world.y - m_vertices[j].y )
                index=j+m_lineLength;
            else if (m_vertices[j+m_lineLength].y - mouse_world.y >  mouse_world.y - m_vertices[j].y )
                index=j;
        }
    }

    return index;
}

#define FUNCTIONS(prim) \
    begin = &DTM::build_begin<prim>; \
    line  = &DTM::build_line <prim>; \
    back  = &DTM::build_back <prim>; \
    end   = &DTM::build_end  <prim>

#define CASE(prim) case prim: FUNCTIONS(prim); break

#define SWITCH_PRIM \
    const Primitive primitive = getPrimitive(); \
    switch(primitive) \
    { \
    CASE(POINT); \
    CASE(TRILINE); \
    CASE(QUADLINE); \
    CASE(TRIFILL); \
    CASE(QUADFILL); \
    CASE(LINELOOP); \
    CASE(TRIANGLES); \
    default: \
        throw UnknownPrimitive(primitive); \
    }

#define BUILD \
    (this->*begin)(); \
    const unsigned long vLength = m_vertices.size(); \
    for(unsigned long i = 0 ; i < vLength ; ++i) \
    { \
        if(i % m_lineLength == 0 && primitive & DESIGN_EDGE) \
        /* beginning of line        only if drawing edges */ \
        { \
            for(unsigned long j = i + m_lineLength-1 ; \
                j >= i && j < vLength ; /* security: j is unsigned so 0 - 1 >= 0 */ \
                --j) \
            { \
                (this->*back)(j); \
            } \
        } \
        (this->*line)(i); \
    } \
    (this->*end)()

void DTM::buildArrays()
{
    const unsigned long arraySize = array_size(getPrimitive());
    prepareBuild(arraySize);

    void (DTM::*begin)() = NULL;
    void (DTM::*line)(unsigned long) = NULL;
    void (DTM::*back)(unsigned long) = NULL;
    void (DTM::*end)() = NULL;

    SWITCH_PRIM;
    BUILD;
}

template<class Child>
void Drawable<Child>::build(Child &ths) const
{
}


/*
 * GENERAL
 */

template<class Child>
inline void Drawable<Child>::build_begin()
{
    switch(m_mode)
    {
    case MODE_LEGACY:
        glBegin(glPrimitive(m_prim));
        break;
    case MODE_VERTEX_ARRAY:
        break;
    case MODE_VERTEX_INDICES:
        break;
    }
}

template<class Child>
inline void Drawable<Child>::build_line(unsigned long) {}

template<class Child>
inline void Drawable<Child>::build_back(unsigned long) {}

template<class Child>
inline void Drawable<Child>::build_end()
{
    switch(m_mode)
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

/*
 * POINTS
 */

inline void DTM::build_line_POINT(unsigned long i)
{
    buildFunction(i);
}

inline unsigned long DTM::array_size_POINT() const
{
    return m_vertices.size();
}


/*
 * EDGES
 */

inline void DTM::build_line_TRILINE(unsigned long i)
{
    buildFunction(i);
    if(i < m_vertices.size() - m_lineLength) // not at last line
        buildFunction(i + m_lineLength);
}

inline void DTM::build_back_TRILINE(unsigned long j)
{
    buildFunction(j);
}

inline unsigned long DTM::array_size_TRILINE() const
{
    return m_vertices.size() + (m_vertices.size() - m_lineLength) // line
            + m_vertices.size();  // back
}



inline void DTM::build_line_QUADLINE(unsigned long i)
{
    build_line_TRILINE(i);
}

inline void DTM::build_back_QUADLINE(unsigned long j)
{
    if(j % m_lineLength) { // not at beginning of line
        buildFunction(j);
        buildFunction(j-1);
    }
}

inline unsigned long DTM::array_size_QUADLINE() const
{
    return m_vertices.size() + (m_vertices.size() - m_lineLength) // line
            + 2*(m_vertices.size() - m_nbLines); // back
}



/*
 * SHAPES
 */

inline void DTM::build_line_TRIFILL(unsigned long i)
{
    buildFunction(i);
    if(i < m_vertices.size() - m_lineLength) // not at last line
        buildFunction(i + m_lineLength);
    else
        buildFunction(i);
}

inline unsigned long DTM::array_size_TRIFILL() const
{
    return 2 * m_vertices.size();
}


inline void DTM::build_line_QUADFILL(unsigned long i)
{
    build_line_TRIFILL(i);
}

inline unsigned long DTM::array_size_QUADFILL() const
{
    return array_size_TRIFILL();
}




/*
 * LINE LOOPS
 */

// Line loops are opened and closed at each time
inline void DTM::build_begin_LINELOOP() {}
inline void DTM::build_end_LINELOOP  () {}

inline void DTM::build_line_LINELOOP(unsigned long i)
{
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
}

inline unsigned long DTM::array_size_LINELOOP() const
{
    if(getMode() == MODE_LEGACY)
        return m_vertices.size()
                + (m_vertices.size() - m_nbLines)
                + (m_vertices.size() - m_lineLength);
    else
        return 0;
}



/*
 * TRIANGLES
 */

inline void DTM::build_line_TRIANGLES(unsigned long i)
{
    if(i % m_lineLength && i < m_vertices.size() - m_lineLength) { // not at beginning nor last line
        buildFunction(i-1);
        buildFunction(i-1 + m_lineLength);
        buildFunction(i);

        buildFunction(i);
        buildFunction(i + m_lineLength);
        buildFunction(i-1 + m_lineLength);
    }
}

inline unsigned long DTM::array_size_TRIANGLES() const
{
    return 6*(m_vertices.size() - m_lineLength - (m_nbLines-1));
}
