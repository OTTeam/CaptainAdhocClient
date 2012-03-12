#include "FileReceivedModel.h"

#include "SimsProtocole/FileStreamer.h"

#include <QStringList>

FileReceivedModel::FileReceivedModel( FileStreamer const * fileStreamer )
{
    m_size = fileStreamer->fileSize();

    // extracting name and type
    m_name = fileStreamer->fileName();

    m_type = m_name.split( '.' )[ 1 ];

    m_progress = 0;

    connect( fileStreamer, SIGNAL( progressUpdate( quint64, float ) ),
             this,   SLOT( setProgress( quint64 ) ) );
    connect( fileStreamer, SIGNAL( progressUpdate( quint64,float ) ),
             this, SLOT( setSpeed( float ) ) );
}
