#ifndef SIMPLEFILEMODEL_H
#define SIMPLEFILEMODEL_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <iostream>

class SimpleFileModel
{
public:
    SimpleFileModel();
    SimpleFileModel(int id, const QString& name, const QString& path, const QString& type, int size, const QString& hash);

    int id() const { return _id; }
    void setId(int id) { _id = id; }

    const QString& name() const { return _name; }
    void setName(const QString& name) { _name = name; }

    const QString& path() const { return _path; }
    void setPath(const QString& path) { _path = path; }

    const QString& type() const { return _type; }
    void setType(const QString& type) { _type = type; }

    int size() const { return _size; }
    void setSize(int size) { _size = size ; }

    const QString& hash() const { return _hash; }
    void setHash(const QString& hash) { _hash = hash; }

    QString toString() const;

    QDataStream& readFromStream(QDataStream& in);

private:
    int _id;
    QString _name;
    QString _path;
    QString _type;
    int _size;
    QString _hash;
};

std::ostream& operator<<(std::ostream& out, const SimpleFileModel& model);

QDebug operator<<(QDebug dbg, const SimpleFileModel& model);

QDataStream &operator<<(QDataStream &, const SimpleFileModel &);

QDataStream &operator>>(QDataStream &, SimpleFileModel &);

#endif // SIMPLEFILEMODEL_H
