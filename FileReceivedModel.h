#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <QObject>

#include "SimsProtocole/utils.h"

class FileStreamer;
class Client;

class FileReceivedModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(int size READ size)
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    FileReceivedModel( FileStreamer const * fileStreamer );
    FileReceivedModel( QString name, quint64 size, HashType hash, Client * father );

    QString name() const { return m_name; }
    QString type() const { return m_type; }
    int size() const { return m_size; }
    int progress() const { return m_progress; }
    float speed() const { return m_speed; }
    HashType hash() const { return m_hash; }

public slots:
    void setName( QString const & name )
    {
        m_name = name;
        emit nameChanged();
    }
    void setType( QString const & type )
    {
        m_type = type;
        emit nameChanged();
    }
    void setProgress( quint64 curSize )
    {
        qDebug() << "[FILEMODEL] Received progress update";
        m_progress = curSize / m_size;
        emit progressChanged();
    }
    void setSpeed( float speed )
    {
        m_speed = speed;
        emit speedChanged();
    }

    void speedAndProgUpdate( quint64 size, float speed )
    {
        setProgress( size );
        setSpeed( speed );
    }

    Q_INVOKABLE void requestDownloadFromView()
    {
        emit downloadRequested( m_hash );
    }

signals:
    void nameChanged();
    void typeChanged();
    void progressChanged();
    void speedChanged();
    void downloadRequested( HashType hash );

private:
    QString m_name;
    QString m_type;
    quint64 m_size;
    int m_progress;
    float m_speed;
    HashType m_hash;
};

#endif // FILEOBJECT_H
