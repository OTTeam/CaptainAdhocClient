#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <QObject>

class Client;

class FileReceivedModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(quint64 size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    FileReceivedModel( Client const * );

    QString name() const { return m_name; }
    QString type() const { return m_type; }
    quint64 size() const { return m_size; }
    int progress() const { return m_progress; }
    int speed() const { return m_speed; }

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
    void setSize( quint64 size )
    {
        m_size = size;
        emit sizeChanged();
    }
    void setProgress( int progress )
    {
        m_progress = progress;
        emit sizeChanged();
    }
    void setSpeed( int speed )
    {
        m_speed = speed;
        emit speedChanged();
    }

signals:
    void nameChanged();
    void typeChanged();
    void sizeChanged();
    void progressChanged();
    void speedChanged();

private:
    QString m_name;
    QString m_type;
    quint64 m_size;
    int m_progress;
    int m_speed;

};

#endif // FILEOBJECT_H
