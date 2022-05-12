#ifndef MD5HASHFILE_H
#define MD5HASHFILE_H

#include <QMap>
#include <QStack>
#include <QObject>

class MD5HashFile : public QObject
{
    Q_OBJECT
public:
    explicit MD5HashFile(QObject *parent = nullptr);
    QString md5_file(QString filename);

signals:
    void appendFileHash(QString hash, QString path);

public slots:
    void start_MD5_hashing(QStack<QString> files);

};


#endif // MD5HASHFILE_H
