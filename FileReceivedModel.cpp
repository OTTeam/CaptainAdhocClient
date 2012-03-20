#include "FileReceivedModel.h"

#include "SimsProtocole/FileStreamer.h"
#include "SimsProtocole/Client.h"

#include <QStringList>

FileReceivedModel::FileReceivedModel( FileStreamer const * fileStreamer )
{
    m_size = fileStreamer->fileSize();

    // extracting name and type
    m_name = fileStreamer->fileName();

    m_type = m_name.split( '.' ).last();

    m_progress = 0;

    connect( fileStreamer, SIGNAL( progressUpdate( quint64, float ) ),
             this,   SLOT( setProgress( quint64 ) ) );
    connect( fileStreamer, SIGNAL( progressUpdate( quint64,float ) ),
             this, SLOT( setSpeed( float ) ) );
}

FileReceivedModel::FileReceivedModel( QString name, quint64 size, HashType hash, Client * father ) :
    m_name( name ),
    m_size( size ),
    m_hash( hash ),
    m_progress( 0 ),
    m_speed( 0 )
{
    m_type = m_name.split( '.' )[ 1 ];

    connect( this, SIGNAL( downloadRequested( HashType ) ),
             father, SLOT( RequestFile( HashType ) ) );
}
