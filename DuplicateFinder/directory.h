#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QDialog>
#include <QObject>
#include <QStack>
#include <QThread>

class Directory : public QThread
{
    Q_OBJECT

private:
    char introducedDir[PATH_MAX];
    char option[PATH_MAX];

public:
    explicit Directory(QThread *parent = nullptr);
    ~Directory();

    void setDirectrory(char* _directory);


public slots:
    void directoryScan(const char* _introducedDir, char* _option, int _fl);

signals:
    void errorExistDirect();
    void appendPath(const QString pathName);


    // QThread interface
protected:
    virtual void run();
};

#endif // DIRECTORY_H
