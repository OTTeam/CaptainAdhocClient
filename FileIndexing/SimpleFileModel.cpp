#include "SimpleFileModel.h"

SimpleFileModel::SimpleFileModel()
{
}

SimpleFileModel::SimpleFileModel(int id, const QString& name, const QString& path, const QString &type, int size, const QString &hash) :
    _id(id), _name(name), _path(path), _type(type), _size(size), _hash(hash)
{
}

QString SimpleFileModel::toString() const
{
    QString str = QString("SimpleFileModel { id = %1, name = %2, path = %3, type = %4, size = %5, hash = %6 }").
            arg(_id).arg(_name).arg(_path).arg(_type).arg(_size).arg(_hash);
    return str;
}

QDataStream& SimpleFileModel::readFromStream(QDataStream& in)
{
    QByteArray byteArray;

    in >> _id;
    in >> byteArray;
    _name = QString(byteArray);
    in >> byteArray;
    _path = QString(byteArray);
    in >> byteArray;
    _type = QString(byteArray);
    in >> _size;
    in >> byteArray;
    _hash = QString(byteArray);
    return in;
}

QDebug operator<<(QDebug dbg, const SimpleFileModel& model)
{
    dbg.nospace() << model.toString();

    return dbg.space();
}

std::ostream& operator<<(std::ostream& out, const SimpleFileModel& model)
{
    out << model.toString().toStdString();
    return out;
}

QDataStream &operator<<(QDataStream& out, const SimpleFileModel& model)
{
    out << (quint32) model.id() << model.name().toAscii() << model.path().toAscii() << model.type().toAscii() << (quint32) model.size() << model.hash().toAscii();
    return out;
}

QDataStream &operator>>(QDataStream& in, SimpleFileModel& model)
{
    return model.readFromStream(in);
}
