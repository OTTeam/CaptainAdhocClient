#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <QObject>

class FileObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)

public:
    FileObject( QString const & name, QString const & type, int size, int progress = -1 );

    QString name() const { return m_name; }
    QString type() const { return m_type; }
    int size() const { return m_size; }
    int progress() const { return m_progress; }

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
    void setSize( int size )
    {
        m_size = size;
        emit sizeChanged();
    }
    void setProgress( int progress )
    {
        m_progress = progress;
        emit sizeChanged();
    }

signals:
    void nameChanged();
    void typeChanged();
    void sizeChanged();
    void progressChanged();

private:
    QString m_name;
    QString m_type;
    int m_size;
    int m_progress;

};

#endif // FILEOBJECT_H
