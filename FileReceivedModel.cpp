#include "FileReceivedModel.h"

#include "SimsProtocole/Client.h"

#include <QStringList>

FileReceivedModel::FileReceivedModel( Client const * client )
{
    //m_size = client->ReceivingFileSize();

    // extracting name and type
    //m_name = client->ReceivingFileName();
    m_type = m_name.split( '.' )[ 1 ];

    m_progress = 0;

    connect( client, SIGNAL( DownloadProgressUpdate( int ) ),
             this,   SLOT( setProgress( int ) ) );
    connect( client, SIGNAL( DownloadSpeedUpdate( int ) ),
             this, SLOT( setSpeed(int) ) );
}
