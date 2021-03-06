#ifndef WIDGET_H
#define WIDGET_H

#include "directory.h"
#include "md5HashFile.h"
#include <QFileDialog>
#include <QWidget>
#include <QThread>
#include <QMap>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_browseButton_clicked();
    void on_startScanButton_clicked();

    void on_deleteButton_clicked();

    void on_cancelButton_clicked();

public slots:
    void errorExistDirect();
    void appendPath(const QString pathName);
    void fileHashing();
    void appendFileHash(const QString text, const QString path);
    void createTreeDisplay();


signals:
    void directoryScan(const char *introducedDir, char *option, int fl);
    void start_MD5_hashing(QStack<QString> files);
    void end_MD5_hashing();

private:
    Ui::Widget *ui;
    Directory *direct;
    MD5HashFile *md5Hash;
    QThread *md5Thread;
    unsigned int amountOfFiles;
    QStack<QString> files;
    QMap<QString, QVector<QString>> hashedFiles;
};
#endif // WIDGET_H
