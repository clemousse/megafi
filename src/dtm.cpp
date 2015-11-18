#include "dtm.h"
#include "dtm.inl"

#include <cmath> // std::abs on doubles

#include <QFile>
#include <QDebug>


using namespace megafi;

DTM::DTM(Mode mode, Primitive prim)
    : Drawable(mode, prim),
      m_dataSizeMin(), m_dataSizeMax(),
      m_colorInterv(0.),
      m_lineLength(0),
      m_nbLines(0)

{
}


DTM::DTM(const DTM &other)
    : Drawable(other),
      m_dataSizeMin(other.m_dataSizeMin),
      m_dataSizeMax(other.m_dataSizeMax),
      m_colorInterv(other.m_colorInterv),
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

bool DTM::buildDTM(QString filePath)
{
    bool fOpen = false;
    lock.lockForWrite();
    if(m_vertices.empty())
    {
        fOpen = readDTM(filePath);
        if(fOpen)
        {
            computeDataSize();
            m_colorInterv = (m_dataSizeMax.z-m_dataSizeMin.z)/3;
            computeLineLength();
            lock.unlock();
            lock.lockForRead();
            if(getMode() == MODE_VERTEX_ARRAY || getMode() == MODE_VERTEX_INDICES)
            {
                lock.unlock();
                buildArrays();
            }
            emit arrayRebuilt();
        }
    }
    lock.unlock();
    return fOpen;
}

/* Purpose: read the coordinates from a DTM file and store them in a QVector
 * Arguments:
 *     filePath: path and filename of the file to be read
 * Return: true if the file have been openedbarre de progression infinie qt
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
        Point p;

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
             << ") and (" << m_dataSizeMax.x << ',' << m_dataSizeMax.y << ',' << m_dataSizeMax.z << ")\n";
}

void DTM::computeLineLength()
{
    const unsigned long NOT_COMPUTED = 0;
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
             {
                 //Q_DebugStream::registerQDebugMessageHandler();
                 qWarning() << "Lines" << i << "and" << i+1 << "don't have the same length.\n";
                 //qInstallMessageHandler(0);
             }

             lineLength_prec = m_lineLength;
             m_lineLength = 0;
             ++m_nbLines;
          }
    }

    m_lineLength = lineLength_prec;

    // to append numbres of points in a line and number of lines in the DTM file
    //Q_DebugStream::registerQDebugMessageHandler();
    qWarning() << "Lines are" << m_lineLength << "points long.\n";
    //qInstallMessageHandler(0);

    //Q_DebugStream::registerQDebugMessageHandler();
    qWarning() << "There are" << m_nbLines << "lines in the file.\n";
    //qInstallMessageHandler(0);
}

unsigned long DTM::computeIndex(Point mouse_world) const
{
    lock.lockForRead();

    unsigned long ix = 0;
    unsigned long index = 0;

    // Find the index of the point which coordinates are containes in mouse_world
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
        else if ( (m_vertices[j+m_lineLength].y < mouse_world.y) && (mouse_world.y < m_vertices[j].y) )
        {
            if ( std::abs(m_vertices[j+m_lineLength].y - mouse_world.y)  <  std::abs(mouse_world.y - m_vertices[j].y) )
                index=j+m_lineLength;
            else if (std::abs(m_vertices[j+m_lineLength].y - mouse_world.y) >  std::abs(mouse_world.y - m_vertices[j].y) )
                index=j;
        }
    }

    lock.unlock();

    emit indexComputed(index);
    return index;
}

megafi::Color DTM::computeColor(unsigned long index) const
{
    megafi::Color ret = {{0, 0, 0}};

    if(m_vertices[index].z < (m_colorInterv+m_dataSizeMin.z) && m_vertices[index].z >= m_dataSizeMin.z)
    {
        ret.r = 255 * (m_vertices[index].z-m_dataSizeMin.z)/(m_colorInterv+m_dataSizeMin.z - m_dataSizeMin.z);
        ret.g = 255;


    }

    else if(m_vertices[index].z <= m_dataSizeMax.z && m_vertices[index].z >= (m_colorInterv)+m_dataSizeMin.z)
    {
        ret.r = 255;
        ret.g = 255 - (255 * (m_vertices[index].z - (m_colorInterv + m_dataSizeMin.z))/(m_dataSizeMax.z - (m_colorInterv + m_dataSizeMin.z)));


    }
    else
    {
        ret.r = 255;
        ret.g = 255;

    }

    return ret;
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

#define NB_CALL_LINE m_vertices.size()
#define NB_CALL_BACK m_vertices.size()

void DTM::buildArrays()
{
    lock.lockForWrite();

    {
        const unsigned long arraySize = array_size(getPrimitive());
        prepareBuild(arraySize);
    }

    void (DTM::*begin)() const = NULL;
    void (DTM::*line )(GLuint) = NULL;
    void (DTM::*back )(GLuint) = NULL;
    void (DTM::*end  )() const = NULL;

    SWITCH_PRIM;

    (this->*begin)();
    const GLuint vLength = m_vertices.size();
    for(GLuint i = 0 ; i < vLength ; ++i)
    {
        if(i % m_lineLength == 0)
        /* beginning of line */
        {
            for(GLuint j = i + m_lineLength-1 ;
                j >= i && j < vLength ; /* security: j is unsigned so 0 - 1 >= 0 */
                --j)
            {
                (this->*back)(j);
            }
        }
        (this->*line)(i);
    }
    (this->*end)();

    lock.unlock();
}

void DTM::buildLegacy() const
{
    lock.lockForRead();

    void (DTM::*begin)()       const = NULL;
    void (DTM::*line) (GLuint) const = NULL;
    void (DTM::*back) (GLuint) const = NULL;
    void (DTM::*end)  ()       const = NULL;

    SWITCH_PRIM;

    (this->*begin)();
    const GLuint vLength = m_vertices.size();
    for(GLuint i = 0 ; i < vLength ; ++i)
    {
        if(i % m_lineLength == 0)
        /* beginning of line */
        {
            for(GLuint j = i + m_lineLength-1 ;
                j >= i && j < vLength ; /* security: j is unsigned so 0 - 1 >= 0 */
                --j)
            {
                (this->*back)(j);
            }
        }
        (this->*line)(i);
    }
    (this->*end)();

    lock.unlock();
}


inline unsigned long DTM::array_size(Primitive prim) const
{
    switch(prim)
    {
    case POINT    : return ARRAY_SIZE_POINT    ; break;
    case TRILINE  : return ARRAY_SIZE_TRILINE  ; break;
    case QUADLINE : return ARRAY_SIZE_QUADLINE ; break;
    case TRIFILL  : return ARRAY_SIZE_TRIFILL  ; break;
    case QUADFILL : return ARRAY_SIZE_QUADFILL ; break;
    case LINELOOP : return ARRAY_SIZE_LINELOOP ; break;
    case TRIANGLES: return ARRAY_SIZE_TRIANGLES; break;
    default:
        throw UnknownPrimitive(prim);
    }
}
