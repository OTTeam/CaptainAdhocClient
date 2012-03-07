#include "fileobject.h"

FileObject::FileObject( QString const & name, QString const & type, int size, int progress ) :
    m_name( name ),
    m_type( type ),
    m_size( size ),
    m_progress( progress )
{
}
